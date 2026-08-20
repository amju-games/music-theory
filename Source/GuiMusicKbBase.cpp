// * Amjula music theory *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <DrawAABB.h>
#include <GuiFactory.h>
#include <MessageQueue.h>
#include <StringUtils.h>
#include <Timer.h>
#include "BassPlayMidi.h"
#include "GuiMusicKbBase.h"
#include "MusicEvent.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

//#define MUSIC_KB_DEBUG

// If defined, we allow the user to drag across the keyboard, pressing and releasing
//  keys accordingly.
#define YES_GLISSANDO

//#define YES_ALLOW_SWIPE_TO_SCROLL

//#define DEBUG_SHOW_AABB

namespace Amju
{
GuiMusicKbBase::~GuiMusicKbBase()
{
  // Make sure all keys which were pressed send final key up events
  ReleaseAllKeys();

  // Make sure there are no lingering notes audible.
  KillPlayerNotes(); 
}

void GuiMusicKbBase::Update()
{
  float dt = TheTimer::Instance()->GetDt();

#ifdef YES_ALLOW_SWIPE_TO_SCROLL
  // Scroll keyboard left/right if swiped
  Vec2f pos = GetLocalPos();
  pos += m_vel * dt;

  if (   (m_vel.x > 0 && pos.x > m_desiredX) 
      || (m_vel.x < 0 && pos.x < m_desiredX))
  {
    m_vel.x = 0;
    pos.x = m_desiredX;
  }

  SetLocalPos(pos);
#endif 

  // Update key angles
  const float m_rotVel = 90.0f;
  for (PKey key : m_keys)
  {
    if (key->m_angle < key->m_desiredAngle)
    {
      key->m_angle += m_rotVel * dt;
      if (key->m_angle > key->m_desiredAngle)
      {
        key->m_angle = key->m_desiredAngle;
      }
    }
    else if (key->m_angle > key->m_desiredAngle)
    {
      key->m_angle -= m_rotVel * dt;
      if (key->m_angle < key->m_desiredAngle)
      {
        key->m_angle = key->m_desiredAngle;
      }
    }
  }
}
 
void GuiMusicKbBase::SetPalette(RCPtr<Palette> palette)
{
  m_palette = palette;
}

void GuiMusicKbBase::ColouriseKeys(std::vector<int> midiNotes)
{
  // Reset all keys to their natural colour
  for (auto& key : m_keys)
  {
    key->m_colour = key->m_naturalColour; // a member function would be nice
  }

  if (!m_palette)
  {
    return;
  }

  for (int midiNote : midiNotes)
  {
    Key* key = GetKey(midiNote);
    if (key)
    {
      key->m_colour = m_palette->GetColour(midiNote);
    } 
  }
}

void GuiMusicKbBase::Key::Press()
{
  if (m_isPressed)
  {
    return;
  }

  m_isPressed = true;
  m_desiredAngle = 5.0f;

  int vol = 100; // TODO Humanise
  PlayMidi(m_midiNote, vol); 

  TheMessageQueue::Instance()->Add(new MusicKbMsg(MusicKbEvent(m_midiNote, true)));

#ifdef MUSIC_KB_DEBUG
  std::cout << "Playing note: " << m_midiNote << "\n";
#endif
}

void GuiMusicKbBase::Key::Release()
{
  if (!m_isPressed)
  {
    return;
  }

  m_isPressed = false;
  m_desiredAngle = 0.0f;

  PlayMidi(m_midiNote, 0); // ?

  TheMessageQueue::Instance()->Add(new MusicKbMsg(MusicKbEvent(m_midiNote, false)));

#ifdef MUSIC_KB_DEBUG
  std::cout << "Releasing note: " << m_midiNote << "\n";
#endif
}

void GuiMusicKbBase::ReleaseKey(Key* key)
{
  if (key == m_lastKey)
  {
    m_lastKey = nullptr;
  }

  if (key)
  {
    key->Release();
  }
}

void GuiMusicKbBase::PressKey(Key* key)
{
  if (!key)
  {
    ReleaseAllKeys(); // ?
    return;
  }

  if (m_lastKey == key)
  {
    return;
  }

  // Don't do this, it prevents polyphony
//  ReleaseKey(m_lastKey);

  m_lastKey = key;

  if (IsVisible())
  {
    key->Press();
  }
}

void GuiMusicKbBase::ReleaseAllKeys()
{
  m_lastKey = nullptr;
  for (Key* pkey : m_keys)
  {
    pkey->Release();
  }
}

int GuiMusicKbBase::GetMinKey() const
{
  Assert(!m_keys.empty());
  int minKey = m_keys[0]->m_midiNote;
  return minKey;
}

int GuiMusicKbBase::GetMaxKey() const
{
  Assert(!m_keys.empty());
  int maxKey = m_keys.back()->m_midiNote;
  return maxKey;
}

int GuiMusicKbBase::GetMinKeyOnScreen() const
{
  return m_onScreenMin;
}

int GuiMusicKbBase::GetMaxKeyOnScreen() const
{
  return m_onScreenMax;
}

GuiMusicKbBase::Key* GuiMusicKbBase::GetKey(int midiNote) const
{
  // Binary search for key with the given midi note value
  //  (keys are in midi note value order)

  // TODO If keys are in midi note order, but don't start at zero, we
  //  can just use the offset to get the index and index into the container,
  //  no??!?!?

  auto it = std::lower_bound(m_keys.begin(), m_keys.end(), midiNote,
    [](const Key* k1, int m) { return k1->m_midiNote < m; }
  );
  if (it == m_keys.end())
  {
    return nullptr;
  }
  Key* key = *it;
  Assert(key);
  // If midiNote value is too low, we get the first key, so check we got the
  //  key we asked for!
  if (key->m_midiNote != midiNote)
  {
    return nullptr;
  }
  return key;
}

float GuiMusicKbBase::GetKeyMidX(int midiKey) const
{
  Assert(midiKey >= 0 && midiKey <= 127);
  const auto key = GetKey(midiKey);
  Assert(key); // make sure midiKey is a valid note?
  const auto& rect = key->m_projectedRect;
  return rect.GetCentre().x;
}

GuiMusicKbBase::Key* GuiMusicKbBase::PickKey(const Vec2f& pos)
{
  std::vector<PKey> pickedKeys; // should be 0, 1 or 2, right?!
  for (PKey& key : m_keys)
  {
    if (key->m_projectedRect.IsPointIn(pos))
    {   
      pickedKeys.push_back(key); 
    }   
  }

  int n = static_cast<int>(pickedKeys.size());
  if (n == 0)
  {
    return nullptr;
  }
  if (n == 1)
  {
    return pickedKeys[0];
  }

  // Decide which one to return: for our keyboard, the black key wins
  for (PKey& key : pickedKeys)
  {
    //auto key = dynamic_cast<GuiMusicKb::Key3d*>(pkey.GetPtr());
    if (key->m_isBlack)
    {   
      return key;
    }   
  }

  // Overlapping white keys: choose the one we are closest to the centre of, just in x axis
  Assert(pickedKeys.size() > 1); // from logic above
  float dist0 = pickedKeys[0]->m_projectedRect.GetCentre().x - pos.x;
  float dist1 = pickedKeys[1]->m_projectedRect.GetCentre().x - pos.x;
  return (dist0 < dist1) ? pickedKeys[0] : pickedKeys[1];
}

bool GuiMusicKbBase::OnCursorEvent(const CursorEvent& ce)
{
#ifdef YES_ALLOW_SWIPE_TO_SCROLL

  if (m_tapDownScroll)
  {
    // Detect swipe
    const float SWIPE_LIMIT = 0.05f; // TODO CONFIG
    const float SPEED_MULT = 16.0f;
    const float OCTAVE_WIDTH = 0.7f;

    float currentX = GetLocalPos().x;
    float dx = ce.dx; 

    if (fabs(dx) > SWIPE_LIMIT && m_vel.x == 0)
    {
      m_vel = Vec2f(dx * SPEED_MULT, 0);
      if (dx > 0)
      {
        // Swipe right => make lower part of the KB visible
        m_desiredX = currentX + OCTAVE_WIDTH;
        float x = m_kbWidth + GetParent()->GetLocalPos().x + 0.2f;
        m_desiredX = std::min(x, m_desiredX); // so we can't go off bottom end
      }
      else
      {
        // Swipe left => make higher part of the KB visible
        m_desiredX = currentX - OCTAVE_WIDTH;
        float x = GetParent()->GetLocalPos().x + 2.1f;
        m_desiredX = std::max(x, m_desiredX); // so we can't go off top end
      }
    }
    return true;
  }
#endif // YES_ALLOW_SWIPE_TO_SCROLL

#ifdef YES_GLISSANDO
  
  // Glissando
  m_tapDownPos = Vec2f(ce.x, ce.y);
  MoveClosestFinger(m_tapDownPos);

#endif // YES_GLISSANDO

  return false;
}

bool GuiMusicKbBase::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  // TODO CONFIG
  // Anything below this line is treated as a tap on the keyboard.
  // Above this, we use to scroll L/R
  const float KEYBOARD_TOP_Y_COORD = -0.2f;

  if (mbe.button != AMJU_BUTTON_MOUSE_LEFT)
  {
    return false;
  }
  
  // Find key we pressed or released
  
  // Touch down on key or in scroll area?
  if (mbe.isDown && mbe.y >= KEYBOARD_TOP_Y_COORD)
  {
    m_tapDownScroll = true;
  }
  else if (mbe.isDown && mbe.y < KEYBOARD_TOP_Y_COORD)
  {
    m_tapDown = true;

    // Store most recent tap down pos: useful for glissando..?
    m_tapDownPos = Vec2f(mbe.x, mbe.y);
    AddFinger(m_tapDownPos);
  }

  if (!mbe.isDown)
  {
    auto tapDownPos = Vec2f(mbe.x, mbe.y);
    EraseClosestFinger(tapDownPos);

    m_tapDown = false;
    m_tapDownScroll = false;
  }

  return false;
}

int GuiMusicKbBase::CountFingersOnKey(Key* key) const 
{
  return static_cast<int>(std::count_if(m_fingers.cbegin(), m_fingers.cend(),
    [=](const Finger& f) { return f.m_key == key; }));
}

// Touch down event: add a new finger
void GuiMusicKbBase::AddFinger(const Vec2f& pos)
{
  Finger f(pos);
  f.m_key = PickKey(pos);
  if (f.m_key)
  {
#ifdef MUSIC_KB_DEBUG
    std::cout << "Adding a finger to key " << f.m_key->m_midiNote << "\n";
#endif
    
    if (CountFingersOnKey(f.m_key) == 0)
    {
      PressKey(f.m_key); // Not if another finger is already on it
    }
    m_fingers.push_back(f);
  }
}
  
// Touch up event: remove finger
void GuiMusicKbBase::EraseClosestFinger(const Vec2f& pos)
{
  auto it = FindClosestFinger(pos);
  if (it != m_fingers.end())
  {
#ifdef MUSIC_KB_DEBUG
    std::cout << "Erasing finger from key " << it->m_key->m_midiNote << "\n";
#endif
    
    if (CountFingersOnKey(it->m_key) == 1)
    {
      ReleaseKey(it->m_key); // Not if another finger is still on it
    }
    m_fingers.erase(it);
  }
}

// Move event: update closest finger
void GuiMusicKbBase::MoveClosestFinger(const Vec2f& pos)
{
  auto it = FindClosestFinger(pos);
  if (it != m_fingers.end())
  {
    it->m_pos = pos; // Update posision
    // Has key changed?
    auto key = PickKey(pos);
    if (key && key != it->m_key)
    {
#ifdef MUSIC_KB_DEBUG
      std::cout << "Moving a finger from " << key->m_midiNote << " to key " << it->m_key->m_midiNote << "\n";
#endif
      
      if (CountFingersOnKey(it->m_key) == 1)
      {
        ReleaseKey(it->m_key); // Not if another finger is still on it
      }
      if (CountFingersOnKey(key) == 0)
      {
        PressKey(key); // Not if another finger is still on it
      }
      it->m_key = key;
    }
  }
}

GuiMusicKbBase::Fingers::iterator GuiMusicKbBase::FindClosestFinger(const Vec2f& pos) 
{
  return std::min_element(m_fingers.begin(), m_fingers.end(),
    [&](const Finger& f1, const Finger& f2) 
    { 
      // Compare squared lengths of vectors
      return (pos - f1.m_pos).SqLen() < (pos - f2.m_pos).SqLen();
    });
}
}


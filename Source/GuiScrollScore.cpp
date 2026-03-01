#include <iostream>
#include <GuiPoly.h>
#include <Timer.h>
#include "GuiScrollScore.h"

//#define ANIM_DEBUG

namespace Amju
{
const char* GuiScrollScore::NAME = "scroll-score";

float GetStartX()
{
  // Sensible position to start from.
  // TODO should be num beats * dist between beats
  return 3.f; // TODO 
}

void GuiScrollScore::DrawChildren()
{
  GuiMusicScore::DrawChildren();

  AmjuGL::PushMatrix();
  AmjuGL::Translate(GetLocalPos().x, 0, 0);
  Batched::DrawAll();
  AmjuGL::PopMatrix();
}

bool GuiScrollScore::LoadMusicScore(File* f)
{
  if (!GuiMusicScore::LoadMusicScore(f))
  {
    return false;
  }
  AddBeatLines();
  m_currentX = GetStartX();
  return true;
}

bool GuiScrollScore::Load(File* f)
{
  if (!GuiMusicScore::Load(f))
  {
    return false;
  }
  AddBeatLines();
  m_currentX = GetStartX();
  return true;
}

void GuiScrollScore::SendNoteEvent(const NoteEvent& ne)
{
  if (m_noteEventCallback)
  {
    m_noteEventCallback(ne);
  }
  // ..but don't play the note
}

void GuiScrollScore::StartCountIn(
  int numCountInBeats, std::function<void()> onFinished)
{
  if (m_noteEvents.empty())
  {
std::cout << "Can't count in, no music events!\n";
    return;
  }

  m_countInFinishedFunc = onFinished;

  // Duration of count-in
  m_countInTimeRemaining = static_cast<float>(numCountInBeats) / GetBpm() * 60.f;

  // 1. We want to move our current position a sensible distance back from the
  //  event we want to hit when the count-in time elapses.
  const float sensible = GetStartX();
  m_currentX = -sensible;

  // 2. Get distance to first music event in score after the count-in.
  // (TODO If counting in to mid-piece, the pos will be of the first event 
  //  we want to play after the count-in, not the beginning of the piece.)
  float d = m_noteEvents.begin()->GetPos().x + sensible;

  // 3. Calc the speed we need to get from our sensible position to the
  //  music event we want to hit when the count-in elapses.
  m_countInSpeed = d / m_countInTimeRemaining;

std::cout << "Count in..... time remaining: " << m_countInTimeRemaining 
  << " dist: " << d 
  << " initial pos: " << m_currentX
  << " vel: " << m_countInSpeed
  << "\n";
}

float GuiScrollScore::GetCountInTimeRemaining() const
{
  return m_countInTimeRemaining;
}

void GuiScrollScore::Update()
{
  // Sigh, can't update here because count-in won't work.
}

void GuiScrollScore::Draw()
{
  // Mini-update in Draw :( ... 
  // We want to update scroll pos when not animating,
  //  when unfortunately Update will not be called.

  // If counting in, scroll without animating
  if (m_countInSpeed > 0)
  {
    const float dt = TheTimer::Instance()->GetDt();
    m_currentX += dt * m_countInSpeed;
 
    m_countInTimeRemaining -= dt;
    if (m_countInTimeRemaining <= 0)
    {
      // Notify client that count in has finished
      m_countInFinishedFunc();

      m_countInSpeed = 0;
      m_countInTimeRemaining = 0;
    }
  }

  auto pos = GetLocalPos();
  const float scale = GetSize().x * GetCombinedScale().x;

  // We want increasingly negative x to scroll R -> L
  pos.x = - m_currentX * scale; 

  SetLocalPos(pos);

  GuiMusicScore::Draw();
}

void GuiScrollScore::Animate(float animValue) 
{
// We don't want to do this, but if we did, we would need to not overwrite
//  the colour of glyphs.
//  GuiMusicScore::Animate(animValue);

  // We do want to update note events to call the callback when they occur.
  UpdateNoteEvents(animValue);

  m_animTime = animValue; 

  auto noteOnEvents = m_noteEvents;
  // Just retain note on events 
  // No, we want rests too

  noteOnEvents.erase(
    std::remove_if(noteOnEvents.begin(), noteOnEvents.end(),
    [](const NoteEvent& ne) 
    { 
      return !ne.IsNoteOnEvent() && !ne.IsRestOnEvent(); 
    }),
    noteOnEvents.end());


  // Find the next entry in the note events sequence
  auto it = std::lower_bound(noteOnEvents.begin(), noteOnEvents.end(), animValue,
    [](const NoteEvent& ne, float f) { return ne.m_time < f; });
  if (it != noteOnEvents.begin()) // go to event before anim time if it exists
  {
    --it;
  }

  if (it == noteOnEvents.end())
  {
    // Have we reached the end of the piece?
std::cout << "End of the piece?\n";
  }
  else
  {
    const float time = it->m_time;
    const float x = it->GetPos().x;
    if (time > m_nextT)
    {
      // We have reached the beat. Recalc velocity to reach the next one.
#ifdef ANIM_DEBUG    
std::cout << "t: " << animValue 
  << " reached: " << time 
  << " old x: " << m_currentX  
  << " new x: " << x; 
#endif
      if (m_currentX < x)
      {
        // Catch up if we have to - but don't jump back, that will look bad. 
        m_currentX = x; 
      }
      m_nextT = time;

      // Find the next time
      ++it;
      if (it == noteOnEvents.end())
      {
        //m_scrollSpeed = 0; // Keep scrolling
std::cout << " - end?\n";
      }
      else
      {
        const float nextTime = it->m_time;
        const float nextX = it->GetPos().x;

        // We want to go from current x to next x in (nextTime - time) normalized time.
        float dt = nextTime - time;
        Assert(dt >= 0);
        Assert(dt <= 1.f);
        float dx = nextX - m_currentX;
        m_scrollSpeed = dx / dt;

#ifdef ANIM_DEBUG
std::cout << " nextT: " << nextTime 
  << " nextX: " << nextX
  << " vel: " << m_scrollSpeed << " units/sec\n";
#endif
      }
    }
  }

  // Scroll from right to left
  // Get 'dt' for the animTime. TODO Should be passed in as a param.
  static float prevAnimValue = animValue;
  float dt = animValue - prevAnimValue;
  if (dt < 0)
  {
    dt = 0;
  }
  prevAnimValue = animValue;
  float dx = m_scrollSpeed * dt; 
  if (dx > 0)
  {
    m_currentX += dx;
  }
}

void GuiScrollScore::OnResetAnimation() 
{
  // Hmm, do we want to call this?
  GuiMusicScore::OnResetAnimation();

  m_animTime = 0; 

  m_nextT = 0;
  m_currentX = -GetStartX();
  m_scrollSpeed = 0;

  auto pos = GetLocalPos();
  pos.x = GetStartX();
  SetLocalPos(pos);
}

void GuiScrollScore::AddBeatLines()
{
#ifdef ADD_BEAT_LINES
  // Add lines to show beat table x values. 
  // They show up a bit to the left of the music glyphs :(
  for (const auto& ne : m_noteEvents)
  {
    auto poly = new GuiPoly;
    poly->SetLocalPos({0, 0});
    poly->AddControlPoint({x, 1.f});
    poly->AddControlPoint({x, -1.f});
    poly->OnControlPointsChanged();
    poly->SetFilledColour(*FromHexString("ff8080"));
    poly->SetOutlineColour(*FromHexString("ff8080"));
    poly->SetStyle(GuiPoly::Style::OUTLINE);
    m_children.push_back(poly);
    m_children.push_back(new GuiFlush);
  }
#endif
}

float GuiScrollScore::GetAnimTime() const
{ 
  return m_animTime;
}
}


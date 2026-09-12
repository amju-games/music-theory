#include <GuiSprite.h>
#include <MessageQueue.h>
#include "GuiMusicKbBase.h"
#include "KeyInputHandler.h"
#include "MusicEvent.h"
#include "QwertyOverlay.h"

namespace Amju
{
static void RegisterKey(char ch, int midiPitch, KeyInputHandler& kih, bool down)
{
  const bool OVERWRITE = true; // trash any existing mappings

  bool added = kih.AddHandler(MakeKeyEvent(ch, down),
    [=](const KeyEvent& ke) 
    { 
      TheMessageQueue::Instance()->Add(
        new MusicKbMsg(MusicKbEvent(midiPitch, down)));
      return true; 
    },
    "Qwerty key for MIDI " + std::to_string(midiPitch),
    OVERWRITE);

  Assert(added);
}

void QwertyOverlay::RegisterKeyEvents(KeyInputHandler& kih)
{
  // Register a callback for each key with the Key Input Handler.
  // KeyEvents for our given keys will be mapped to our handler.

  // TODO This isn't right, we want to find the actual midi notes
  //  we are covering... unless we just don't play bum notes.
  const int midiPitchStart = 60;
  for (int i = 0; i < 12; i++)
  {
    auto s = GetQwertyStrForMidi(i);
    char ch = s[0];
    const bool KEY_UP = false;
    const bool KEY_DOWN = true;
    // Register two KeyEvents: one for key up, one for key down.
    RegisterKey(ch, midiPitchStart + i, kih, KEY_UP);
    RegisterKey(ch, midiPitchStart + i, kih, KEY_DOWN);
  }
}

void QwertyOverlay::Reset()
{
  m_qwertyRoot.Reset();
}

void QwertyOverlay::SetVisible(bool visible)
{
  Assert(m_qwertyRoot);
  if (m_qwertyRoot)
  {
    m_qwertyRoot->SetVisible(visible);
  }
}

std::string QwertyOverlay::GetQwertyStrForMidi(int midi) const
{
  // TODO This only works for US/UK keyboards
  const auto keyStrs = "zsxdcvgbhnjm";
  return std::string(1, keyStrs[midi % 12]);
}

void QwertyOverlay::SetQwertyStrForMidi(WeakPtr<GuiElement> key, int midi)
{
  Assert(key);
  auto str = GetQwertyStrForMidi(midi);
  auto text = GetElementByName(key, "qwerty-key-text");
  GuiSprite* sprite = dynamic_cast<GuiSprite*>(text);
  if (sprite)
  {
    sprite->SetCell(static_cast<int>(str[0]) - 32); 
  }
}

bool QwertyOverlay::Init(WeakPtr<GuiElement> root)
{
  m_qwertyRoot = dynamic_cast<GuiComposite*>(
    GetElementByName(root, "qwerty-root"));
  if (!m_qwertyRoot)
  {
    std::cout << "Failed to find root for qwerty overlay!\n";
    return false;
  }
  return true;
}

WeakPtr<GuiElement> QwertyOverlay::GetQwertyKeyForMidi(int midi)
{
  std::string keyName = "qwerty-" + std::to_string(midi);
  RCPtr<GuiElement> key = m_qwertyRoot->GetElementByName(keyName);
  if (key)
    return key;

  key = LoadGui("Gui/qwerty-key.txt", false);
  key->SetName(keyName);

  m_qwertyRoot->AddChild(key);
  // Set letter/number string
  SetQwertyStrForMidi(key, midi);

  return key;
}

bool QwertyOverlay::SetKeyPositions(GuiMusicKbBase& pianoKeys)
{
  int left = pianoKeys.GetMinKeyOnScreen();
  int right = pianoKeys.GetMaxKeyOnScreen();
  for (int i = left; i <= right; ++i)
  {
    float centreX = pianoKeys.GetKeyMidX(i);
    auto q = GetQwertyKeyForMidi(i);
    const bool isBlack = pianoKeys.GetKey(i)->m_isBlack;
    if (q)
    {
      Vec2f pos(centreX, pianoKeys.GetYForQwerty(isBlack));
      q->SetLocalPos(pos);
    }
  }

  return true; // can this fail?
}
}

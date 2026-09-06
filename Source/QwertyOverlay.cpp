#include <GuiSprite.h>
#include "GuiMusicKbBase.h"
#include "QwertyOverlay.h"

namespace Amju
{
void QwertyOverlay::Clear()
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
  auto key = GetElementByName(m_qwertyRoot, keyName);
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
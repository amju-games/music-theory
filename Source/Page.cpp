// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <ConfigFile.h>
#include <EventPoller.h>
#include <GameState.h>
#include <GuiDecAnimation.h>
#include "Consts.h"
#include "Dictionary.h"
#include "GSPages.h"
#include "Page.h"

namespace Amju
{
static void OnHint(GuiElement* hintButton)
{
  TheGSPages::Instance()->OnHint();
}

// Called from timed FuncMessage
void GoToNextPage()
{
  GSPages* pages = TheGSPages::Instance();
  // Sanity check: this only makes sense if GSPages is active
  if (TheGame::Instance()->GetState() == pages)
  {
    std::cout << "GoToNextPage calling NextPage...\n";

    pages->NextPage();
  }
}

void Page::SendNextPageMessage()
{
  TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage,
    SecondsFromNow(NEXT_PAGE_TIME)));
}

void Page::SetConfigFile(ConfigFile* cf)
{
  m_config = cf;
}

void Page::SetDictionaryName(const std::string& dictionaryName)
{
  m_dictionaryFilename = dictionaryName;
}

Dictionary* Page::GetDictionary()
{
#ifdef _DEBUG
  // Not a resource, so we can easily reload
  Dictionary* dic = new Dictionary;
  bool loaded = dic->Load(m_dictionaryFilename);
  Assert(loaded);
#else
  // Get musical terms dictionary - this is a Resource.
  Dictionary* dic = dynamic_cast<Dictionary*>(
    TheResourceManager::Instance()->GetRes(m_dictionaryFilename + ".dictionary"));
  Assert(dic);
#endif
  return dic;
}

void Page::SetGuiName(const std::string& guiName)
{
  m_guiName = guiName;
}

GuiButton* Page::GetHintButton() const
{
  GuiElement* hint = GetElementByName(TheGSPages::Instance()->GetGui(), "hint-button");
  return dynamic_cast<GuiButton*>(hint);
}

void Page::SetIsEnabled(bool enabled)
{
  if (enabled)
  {
    // Re-enable buttons
    GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(
      GetElementByName(m_gui, "anim-colour-fade-whole-page"));
    Assert(anim);
    anim->ResetAnimation();
    anim->SetIsReversed(true);
  }
  else
  {
    // Fade page contents down, which disables the buttons
    GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(
      GetElementByName(m_gui, "fade-out-whole-page"));
    Assert(anim);
    anim->ResetAnimation();
    anim->SetIsReversed(false);
    anim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);

    anim = dynamic_cast<GuiDecAnimation*>(
      GetElementByName(m_gui, "anim-colour-fade-whole-page"));
    Assert(anim);
    anim->SetIsReversed(false);
  }
}

void Page::OnDeactive()
{
  if (m_gui)
  {
    // Delete old GUI: listener ref prevents its deletion
    TheEventPoller::Instance()->RemoveListener(m_gui);
    m_gui = nullptr;
  }
}

void Page::OnActive()
{
  m_gui = LoadGui("Gui/boilerplate.txt");

  // Load page-specific gui and join it to the boilerplate
  std::string landscape = "_landscape";
  PGuiElement pageGui = LoadGui("Gui/page_" + m_guiName + landscape + ".txt", false);
  Assert(pageGui);

  GuiComposite* addComp = dynamic_cast<GuiComposite*>(GetElementByName(m_gui, "add-page-to-me"));
  Assert(addComp);
  addComp->AddChild(pageGui);

  // Set command for common buttons
  GuiElement* hint = GetHintButton();
  if (hint)
  {
    hint->SetCommand(Amju::OnHint);
  }
}

bool Page::Load(const Strings& strs)
{
  // Topic pages must specify the dictionary to use, as it's no longer hard coded anywhere.
  Assert(strs.size() > 1);
  // 2nd string is dictionary for the page
  SetDictionaryName(strs[1]);
  return true;
}

Question* Page::GetQuestion()
{
  return m_question;
}

GuiElement* Page::GetGui()
{
  return m_gui;
}
 
void Page::SetQuestion(Question* q)
{
  m_question = q;
}

void Page::SetPageQuestion(PageQuestion* pq)
{
  m_pageQuestion = pq;
  m_pageQuestion->SetPage(this);
}

void Page::SetUpQuestionUI()
{
  Assert(m_pageQuestion);
  m_pageQuestion->SetUp();
}

void Page::Draw()
{
  m_gui->Draw();
}

void Page::Update()
{
  m_gui->Update();
}

void Page::SetGameState(GameState* gs)
{
  m_gs = gs;
}

void Page::SetIsLearnMode(bool learn)
{
  m_isLearnMode = learn;
}

bool Page::IsLearnMode() const
{
  return m_isLearnMode;
}
}


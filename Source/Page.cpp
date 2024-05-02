// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <ConfigFile.h>
#include <EventPoller.h>
#include <GameState.h>
#include <GuiDecAnimation.h>
#include <Localise.h>
#include "CentreMsg.h"
#include "Consts.h"
#include "Dictionary.h"
#include "GSPages.h"
#include "Page.h"

namespace Amju
{
static void OnHint(GuiElement* hintButton)
{
  GetPagesState()->OnHint();
}

// Convenience function: get the GSPages game state, but only if it's
//  currently active.
GSPages* GetPagesState()
{
  auto* gameState = TheGame::Instance()->GetState();
  return dynamic_cast<GSPages*>(gameState);
}

// Called from timed FuncMessage
void GoToNextPage()
{
  // Sanity check: this only makes sense if GSPages is active
  if (auto pages = GetPagesState()) 
  {
    pages->NextPage();
  }
}

std::string Page::GetInfo() const
{
  const std::string guiFilename = "Gui/page_" + m_guiName + "_landscape.txt";

  std::string info =
    "Page info: GUI filename: " + guiFilename +
    "\nDictionary filename: " + m_dictionaryFilename +
    "\nInstruction text: " + GetInstructionText();

  if (m_question)
  {
    info +=
      "\nQuestion: " + m_question->GetQuestionString() +
      "\nAnswer: " + m_question->GetAnswerString() +
      "\nExplanation: " + m_question->GetExplanationString();
  }
  else
  {
    info += "\nQuestion: not set";
  }
  info += "\n";
  return info;
}

void Page::SendNextPageMessage()
{
  // As we are transitioning to the next page after a delay, we have
  //  got time to show the blackboard wipe anim here.
  if (auto pages = GetPagesState())
  {
    pages->ShowGuiElement("blackboard-erase");
  }

  TheMessageQueue::Instance()->Add(new FuncMsg(GoToNextPage,
    SecondsFromNow(NEXT_PAGE_TIME)));
}

void Page::SetDictionaryName(const std::string& dictionaryName)
{
  m_dictionaryFilename = dictionaryName;
  LoadDictionary();
}

Dictionary* Page::GetDictionary()
{
  Assert(m_dictionary);
  return m_dictionary;
}

Dictionary* Page::LoadDictionary()
{
  // This looks a bit worrying but is perhaps useful when developing a dictionary?
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
  m_dictionary = dic;
  return dic;
}

void Page::SetGuiName(const std::string& guiName)
{
  m_guiName = guiName;
}

GuiButton* Page::GetHintButton() const
{
  GuiElement* hint = GetElementByName(GetPagesState()->GetGui(), "hint-button");
  return dynamic_cast<GuiButton*>(hint);
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

  // Show instruction text for the page
  // Only show if different. Show in centre, so it doesn't introduce delay,
  //  lets us have long explanations if needed, and we know the player read
  //  it.
  static std::string prevInstr;
  const std::string& instr = GetInstructionText();

  if (instr != prevInstr)
  {
    const float INSTRUCTION_TIME = 1.2f; 
    // TODO CONFIG, or depends on length of text? 

    PLurkMsg lm = new CentreMsg(instr,
      Colour(0, 0, 0, 1), // text colour,
      Colour(1, 1, 1, 1), // bg colour,
      INSTRUCTION_TIME);

    TheLurker::Instance()->Queue(lm);
    prevInstr = instr; // remember for next time
  }
}

bool Page::Load(const Strings& strs)
{
  // Topic pages must specify the dictionary to use, as it's no longer hard coded anywhere.
  Assert(strs.size() > 1);

  // 2nd string is dictionary for the page
  SetDictionaryName(strs[1]);

  // 3rd string is the instruction text
  Assert(strs.size() > 2);
  m_instructionText = Lookup(strs[2]);

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

} // namespace


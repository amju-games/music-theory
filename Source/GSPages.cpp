// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <ConfigFile.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <GuiDecColour.h>
#include <GuiText.h>
#include <Localise.h>
#include <MessageQueue.h>

#include "CentreMsg.h"
#include "Consts.h"
#include "Course.h"
#include "Dictionary.h"
#include "GSMainCorridor.h"
#include "GSPages.h"
#include "GSPagesEdit.h"
#include "GSPause.h"
#include "GSTopicStart.h"
#include "GSTopicEnd.h"
#include "GuiAvatar.h"
#include "MySceneGraph.h"
#include "NetSend.h"
#include "NumUpdate.h"
#include "PlayWav.h"
#include "PrintGui.h"
#include "Topic.h"
#include "UserProfile.h"
#include "UseVertexColourShader.h"

namespace Amju
{
static void GoToEditMode()
{
  TheGSPagesEdit::Instance()->SetPrevState(TheGSPages::Instance());
  TheGame::Instance()->SetCurrentState(TheGSPagesEdit::Instance());
}

static void OnQuitConfirmOK(GuiElement*)
{
  TheGSPages::Instance()->OnQuitConfirmOK();
}

static void OnQuitConfirmCancel(GuiElement*)
{
  TheGSPages::Instance()->OnQuitConfirmCancel();
}
  
static void OnQuitButton(GuiElement*)
{
  TheGSPages::Instance()->OnQuitButton();
}

GSPages::GSPages()
{
  m_guiFilename = "Gui/gs_pages_landscape.txt";
}

QuestionProgress& GSPages::GetProgress()
{
  Assert(m_currentPage < static_cast<int>(m_progress.size()));
  return *(m_progress[m_currentPage]);
}

void GSPages::ShowGuiElement(const std::string& elemName, bool showNotHide)
{
  GuiElement* elem = GetElementByName(m_gui, elemName);
  Assert(elem);
  elem->SetVisible(showNotHide);
}

void GSPages::StartTopic(int topicNum)
{
  m_numPagesShown = 0;
  m_currentPage = 0;
  m_numCorrectThisSession = 0;
  m_numIncorrectThisSession = 0;
  m_scoreThisSession = 0;
  m_scoreMultiplier = 1;
  m_livesThisSession = 3;

  // We get this from Topic later. 
  m_maxNumPagesThisSession = 0;
}

void GSPages::ShowHints()
{
  Assert(m_page);

  // Set GUI to show numbers of hints available
  // Regular Hints
  auto profile = TheUserProfile();
  int hints = profile->GetHints(HintType::HINT_TYPE_HINT);
  NumUpdate(m_gui, "hint-counter" /* TODO CONST */, hints);
  // Disable button if no hints available
  bool enabled = (hints > 0 && m_page->CanGetHint());
  SetButtonEnabled("hint-button", enabled);

  // Play button hints
  GuiButton* playButton = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "play-button"));
  hints = profile->GetHints(HintType::HINT_TYPE_PLAY);
  NumUpdate(m_gui, "play-counter" /* TODO CONST */, hints);
  if (playButton->IsVisible())
  {
    SetButtonEnabled("play-button", playButton->IsEnabled() &&  hints > 0);
  }
}

void GSPages::ReloadGui()
{
  m_gui = LoadGui(m_guiFilename);

  GuiElement* elem = GetElementByName(m_gui, "pause-button");
  Assert(elem);
  elem->SetCommand(Amju::OnQuitButton);

  if (m_page)
  {
    // Reload the page GUI
    m_page->OnDeactive();
    m_page->OnActive();
  }
}

void GSPages::OnActive()
{
  // This WON'T be called if we resume from pause, there is a confirm dialog
  //  in this state.

  GSBase::OnActive();

  // GUI is loaded in GSBase::OnActive. Don't call ReloadGui() to set the quit
  //  button handler, as this will load the entire GUI again.
 
  // Set quit button handler - TODO factor into func
  GuiElement* elem = GetElementByName(m_gui, "pause-button");
  Assert(elem);
  elem->SetCommand(Amju::OnQuitButton);

  // Get general user config, just a convenience, it lives in the User Profile.
  m_userConfig = TheUserProfile()->GetConfigFile();

  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(TheUserProfile()->GetCurrentTopic());
  Assert(topic);
  m_maxNumPagesThisSession = topic->GetNumPages();

  NetSendMarkAttemptStart(topic->GetId());

  // Set up composer avatar. Store Composer for text later.
  m_composer = GetComposerList().GetComposerForTopic(topic->GetId());
  Assert(m_composer);
  GuiAvatar* avatar = dynamic_cast<GuiAvatar*>(GetElementByName(m_gui, "composer-avatar"));
  avatar->SetFromString(m_composer->GetAvatarStr());

  // Create a new, empty container of progress objects.
  // We create one progress object for each page.
  m_maxMark = 0; // calc max mark while we're here
  m_progress.clear();
  m_progress.reserve(m_maxNumPagesThisSession);
  for (int i = 0; i < m_maxNumPagesThisSession; i++)
  {
    Page* page = topic->GetPage(i);
    Assert(page);
    const Dictionary* dic = page->GetDictionary();
    Assert(dic);
    const int numTerms = dic->GetNumTerms();
    m_maxMark += numTerms;
    QuestionProgress* qp = new QuestionProgress;
    qp->SetMaxQuestions(numTerms);
    m_progress.push_back(qp);
  }

  NextPage();
}

void GSPages::OnDeactive()
{
  GSBase::OnDeactive();
  
  SetPage(nullptr);
  
  // Save changes to user profile (questions seen, etc)
  TheUserProfile()->Save();
  // Just in case we switch users or something happens to invalidate this
  m_userConfig = nullptr;

  m_composer = nullptr;
}

void GSPages::UpdateHud()
{
  // Score
  // Show PERCENTAGE
  float percent = static_cast<float>(m_scoreThisSession) / static_cast<float>(m_maxMark) * 100.f;
  NumUpdate(m_gui, "score-text", ToString(static_cast<int>(percent)) + "%");

  // Lives
  NumUpdate(m_gui, "num-lives-text", m_livesThisSession);

  // TODO Combo???
}

bool GSPages::FindPageWithUnusedQuestions()
{
  // Find the next page which has unused questions. If there is no page
  //  with unused questions, return false.

  m_currentPage = (m_currentPage + 1) % m_maxNumPagesThisSession;
  int cp = m_currentPage; // store so we can detect cycle

  Assert(m_currentPage < static_cast<int>(m_progress.size()));

  QuestionProgress* qp = m_progress[m_currentPage];
  while (qp->AllQuestionsUsed())
  {
    m_currentPage = (m_currentPage + 1) % m_maxNumPagesThisSession;
    if (m_currentPage == cp)
    {
      // We have cycled through all the pages, and none have unused questions
      return false;
    }
    qp = m_progress[m_currentPage];
  }

  std::cout << "Page: " << m_currentPage 
    << " - this page has " << qp->GetNumQuestionsUsed() 
    << " questions used of " << qp->GetMaxQuestions() << "\n";

  return true;
}

void GSPages::SetFinalScore()
{
  // Get topic ID
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(TheUserProfile()->GetCurrentTopic());
  Assert(topic);

  // Calc percentage score
  float percent = static_cast<float>(m_scoreThisSession) / static_cast<float>(m_maxMark) * 100.f;
  int s = static_cast<int>(percent);

  TheUserProfile()->SetTopicScore(s, topic->GetId());
}

void GSPages::NextPage()
{
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(TheUserProfile()->GetCurrentTopic());
  Assert(topic);

  // Have we run out of lives?
  if (m_livesThisSession < 1) 
  {
    // Done, go to Topic successfully completed, or unsuccessfully completed.
    // (Can use the same state?)
    SetFinalScore();
    TheGame::Instance()->SetCurrentState(TheGSTopicEnd::Instance());
    float percent = static_cast<float>(m_scoreThisSession) / static_cast<float>(m_maxMark) * 100.f;
    NetSendAttempt(topic->GetId(), NET_SEND_ATTEMPT_NO_LIVES_LEFT, static_cast<int>(percent), 0);
    return;
  }

  // Rotate pages in topic, until we run out of unused questions.
  // Skip pages for which all questions are used.
  if (!FindPageWithUnusedQuestions())
  {
    // No more unused questions
    SetFinalScore();
    TheGame::Instance()->SetCurrentState(TheGSTopicEnd::Instance());
    float percent = static_cast<float>(m_scoreThisSession) / static_cast<float>(m_maxMark) * 100.f;
    NetSendAttempt(topic->GetId(), NET_SEND_ATTEMPT_COMPLETE, static_cast<int>(percent), m_livesThisSession);
    return;
  }

  // The current Topic has sequence of Pages to display.
  Page* page = topic->GetPage(m_currentPage);
  SetPage(page);
  
  m_numPagesShown++;

  // Rub out blackboard - reset anim
  GuiElement* ruboutAnim = GetElementByName(m_gui, "blackboard-erase");
  Assert(ruboutAnim);
  ruboutAnim->SetVisible(false);
  ruboutAnim->ResetAnimation();

  UpdateHud();

  ShowHints();
}

void GSPages::Draw2d()
{
  if (m_page)
  {
    UseVertexColourShader();
    m_page->Draw();
  }

  // Common GUI goes on top or under? We want tick/cross to go on top.
  // We want to draw the avatar Under so avatar doesn't obscure page GUI.
  // But we want to draw the central lurk message last, with the 
  //  darkened bg UNDER the avatar! We don't want to grey out the avatar.
  GSBase::Draw2d();
}

void GSPages::Update()
{
  GSBase::Update();
  if (m_page)
  {
    m_page->Update();
  }
}

// Load list of pages from file?
bool GSPages::Load(const std::string& filename)
{
  return true;
}

bool GSPages::OnKeyEvent(const KeyEvent& ke)
{
  if (GSBase::OnKeyEvent(ke))
  {
    return true;
  }

#ifdef _DEBUG
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR)
  {
    switch (ke.key)
    {
    case '9':
      GoToEditMode();
      return true;

    case 'p':
      // Show page GUI tree
      if (m_page && m_page->GetGui())
      {
        PrintGui(m_page->GetGui());
      }
      else
      {
        std::cout << "Null page GUI!\n";
      }
      break;

    case 'o':
      {
        // Go to topic end
        SetFinalScore();
        TheGame::Instance()->SetCurrentState(TheGSTopicEnd::Instance());
        float percent = static_cast<float>(m_scoreThisSession) / static_cast<float>(m_maxMark) * 100.f;
  
        Course* course = GetCourse();
        Assert(course);
        Topic* topic = course->GetTopic(TheUserProfile()->GetCurrentTopic());
        Assert(topic);

        NetSendAttempt(topic->GetId(), NET_SEND_ATTEMPT_CHEAT, static_cast<int>(percent), m_livesThisSession);
        break;
      }

    case 'i':
        // Inc score
        m_scoreThisSession++;
        UpdateHud();
        break;
    } // switch
  } // if

#endif
  return false;
}

void GSPages::SetPage(Page* p)
{
  if (m_page)
  {
    m_page->OnDeactive();
  }

  m_page = p;

  if (m_page)
  {
    // Activate new page
    m_page->OnActive();
  }
}

void GSPages::OnCorrect()
{
  // Happy sound
  PlayWav(WAV_CORRECT);

  // Add to profile/score
  m_numCorrectThisSession++;

  m_scoreThisSession += m_scoreMultiplier;

  UpdateHud();

  Page::SendNextPageMessage();

  // Customise the message depending on the Composer for this Topic
  PLurkMsg lm = new CentreMsg(m_composer->GetCorrectStr(m_correctStreak),
    Colour(0, 0, 0, 1), // text colour,
    Colour(1, 1, 1, 1), // bg colour,
    1.0f); // TODO CONFIG
    // TODO Speech bubble arrow flag/enum

  TheLurker::Instance()->Queue(lm);
}

void GSPages::OnIncorrect()
{
  m_page->ShowCorrectAnswer();

  // Unhappy sound
  PlayWav(WAV_INCORRECT);

  m_numIncorrectThisSession++;

  m_livesThisSession--;
  UpdateHud();

  // Do this in m_page->ShowCorrectAnswer() if required
  //LurkMsg lm(Lookup("Incorrect!"), 
  //  GetColour(COLOUR_TEXT),
  //  GetColour(COLOUR_INCORRECT),
  //  AMJU_TOP, PAGE_LURK_TIME);
  //TheLurker::Instance()->Queue(lm);
}

void GSPages::OnHint()
{
  // Decrement hints left
  auto profile = TheUserProfile();
  if (profile->GetHints(HintType::HINT_TYPE_HINT) < 1) // TODO
  {
    // No hints sound/anim?
    // TODO

    return;
  }

  if (!m_page->CanGetHint())
  {
    return;
  }

  profile->AddHints(HintType::HINT_TYPE_HINT, -1); // TODO

  m_page->OnHint(); // show context-sensitive hint

  ShowHints();
}

void GSPages::SetButtonEnabled(const std::string& buttonName, bool enabled)
{
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, buttonName));
  Assert(button);
  button->SetIsEnabled(enabled);

  // Set colour (assumes immediate parent is colour decorator)
  GuiElement* col = button->GetParent();
  Assert(col);
  col->Animate(enabled ? 0.f : 1.f); // i.e. enabled = first colour, disabled = 2nd colour
}

void GSPages::OnQuitButton()
{
  // Lurk messages are modal, so no need to diable page
  ShowYesNo(
    // TODO Get this string from the current Composer, e.g.
    //   m_composer->GetConfirmStr()
    Lookup("$$$90") /* Sure you want to quit? */,
    GetConstColour(COLOUR_TEXT),
    GetConstColour(COLOUR_CONFIRM_QUIT),
    Amju::OnQuitConfirmCancel,
    Amju::OnQuitConfirmOK);
}

void GSPages::OnQuitConfirmOK()
{
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(TheUserProfile()->GetCurrentTopic());
  Assert(topic);

  float percent = static_cast<float>(m_scoreThisSession) / static_cast<float>(m_maxMark) * 100.f;

  NetSendAttempt(topic->GetId(), NET_SEND_ATTEMPT_QUIT, static_cast<int>(percent), m_livesThisSession);

  TheGame::Instance()->SetCurrentState(TheGSMainCorridor::Instance());
}

void GSPages::OnQuitConfirmCancel()
{
  // Nothing to do, modal Lurk dialog returns control back to us.
}

void GSPages::OnMusicKbEvent(const MusicKbEvent& e)
{
  if (m_page)
  {
    m_page->OnMusicKbEvent(e);
  }
}

const std::string& GSPages::GetIncorrectStr() const
{
  Assert(m_composer);
  return m_composer->GetIncorrectStr(m_incorrectStreak);
}
}


// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiText.h>
#include <Localise.h>
#include <MessageQueue.h>
#include "Consts.h"
#include "Course.h"
#include "GSMainCorridor.h"
#include "GSTopicEnd.h"
#include "NumUpdate.h"
#include "PlayMidi.h"
#include "Topic.h"
#include "UserProfile.h"

namespace Amju
{
namespace 
{
// We play an ascending scale as the score counter increments.
// Start on this note
const int MIDI_START_NOTE = 65;
// Number of points to show until we play the next note
const int POINTS_PER_NOTE = 8;

void OnOK(GuiElement*)
{
  TheMessageQueue::Instance()->Add(new FuncMsg(
    GoTo<TheGSMainCorridor>, SecondsFromNow(1.0f)));
}

void UpdateNums()
{
  GSTopicEnd* gte = TheGSTopicEnd::Instance();
  if (TheGame::Instance()->GetState() == gte)
  {
    TheGSTopicEnd::Instance()->UpdateNums();
  }
}
} // anon namespace

GSTopicEnd::GSTopicEnd()
{
  m_guiFilename = "Gui/gs_topic_end.txt";
  m_sceneFilename = "Scene/topic_start_scene.txt";
}

void GSTopicEnd::Draw()
{
  GSBase3d::Draw();
}

void GSTopicEnd::SetBestScore(int best)
{
  m_bestScore = best;
}

void GSTopicEnd::OnActive()
{
  GSBase3d::OnActive();

  m_midiNote = MIDI_START_NOTE;

  // Set button commands
  GuiElement* elem = GetElementByName(m_gui, "ok-button");
  elem->SetCommand(OnOK);
  elem->SetHasFocus(true);

  GuiElement* shareButton = GetElementByName(m_gui, "share-button");
  Assert(shareButton);
  shareButton->SetCommand(Amju::OnShare);
  // TODO Set Share message and image
  
  auto profile = TheUserProfile();
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(profile->GetCurrentTopic());
  Assert(topic);

  // Get the non-persistent score for the current attempt. 
  m_topicScore = profile->GetCurrentTopicScore();

  // Unlock next topics if this topic has been passed.
  bool passed = profile->IsCurrentTopicPassed();
  if (passed)
  {
    // Passed this topic, unlock more topics
    const std::vector<std::string>& unlocks = topic->GetTopicsThisUnlocks();
    for (const std::string& id : unlocks)
    {
      profile->UnlockTopic(id);
    }
  }

  // Save all owned config files, but only for those that have changed.
  profile->Save();

  m_totalScore = 0; 

  // Set comment about score etc
  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "comment-text"));
  Assert(text);
  text->SetText(Lookup(GenerateScoreComment()));

  // Set topic display name
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "topic-name-text"));
  Assert(text);
  text->SetText(profile->GetCurrentTopicDisplayName());
  // Also set topic name on certificate
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "cert-topic-name-text"));
  Assert(text);
  text->SetText(profile->GetCurrentTopicDisplayName());
  // Set percent mark on certificate
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "cert-mark"));
  Assert(text);
  text->SetText(ToString(m_topicScore) + "%");
  // Trigger certificate if we have passed
  GuiElement* certificate = GetElementByName(m_gui, "certificate");
  Assert(certificate);
  certificate->SetVisible(passed);

  // Get initial hints, animate additions
  m_hints = profile->GetHints(HintType::HINT_TYPE_HINT); // TODO

  // Add hints now
  int addHints = m_topicScore; // TODO calc num hints to add
  m_finalHints = m_hints + addHints;
  profile->AddHints(HintType::HINT_TYPE_HINT, addHints);  // TODO

  SetScoreNumbers();
  SetHintNumbers();

  // Set best so far %
  // If we just reached the best, show it with particles etc., once our score
  //  has finished incrementing.
  // If we are below the best so far, just show it from the start.

  int best = profile->GetBestTopicScore(topic->GetId());
  if (best > m_bestScore)
  {
    // We have beaten the previous best score!
    Assert(m_topicScore == best); // sanity check
  }
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "best-so-far-num"));
  Assert(text);
  text->SetText(ToString(m_bestScore) + "%");
  if (m_topicScore < best)
  {
    // We didn't beat the best score
    Assert(best == m_bestScore); // sanity check: best score is unchanged
  }

  // Send a timed message to do a number animation
  // First msg is delayed longer than subsequent msgs.
  TheMessageQueue::Instance()->Add(new FuncMsg(::Amju::UpdateNums, SecondsFromNow(1.0f)));
}

std::string GSTopicEnd::GenerateScoreComment()
{
  std::string res = "$$$172" /* Awesome! */;

  auto profile = TheUserProfile();
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(profile->GetCurrentTopic());
  Assert(topic);

  bool passed = profile->IsCurrentTopicPassed();

  if (m_topicScore > 99)
  {
    res = "$$$173" /* Perfect score! */;
  }
  else if (m_topicScore > m_bestScore)
  {
    res = "$$$174" /* Your new best score! */;
  }
  else if (passed)
  {
    res = "$$$175" /* You passed the test! */;
  }
  else
  {
    res = "$$$176" /* Oh no! Try again! */;
  }

  return res;
}

void GSTopicEnd::SetScoreNumbers()
{
  NumUpdate(m_gui, "total-score-text" /* TODO CONST */, 
    ToString(m_totalScore) + "%");

  NumUpdate(m_gui, "best-so-far-num" /* TODO CONST */, 
    ToString(m_bestScore) + "%");
}

void GSTopicEnd::SetHintNumbers()
{
//  NumUpdate(m_gui, "hint-counter" /* TODO CONST */, m_hints);
}

void GSTopicEnd::UpdateNums()
{
  // Set this to true if we need to do another update after this one
  bool updateAgain = true;

  if (m_topicScore > 0)
  {
    bool playNote = false;

    const int incr = 1;
    m_topicScore -= incr;
    updateAgain = true;
    if (m_topicScore <= 0)
    {
      m_topicScore = 0;
      updateAgain = false;
      playNote = true;
    }
    m_totalScore += incr;
    if (m_totalScore % POINTS_PER_NOTE == 0)
    {
      playNote = true;
    }
    if (m_totalScore > m_bestScore)
    {
      m_bestScore = m_totalScore;
    }

    SetScoreNumbers();

    if (playNote)
    {
      PlayMidi(m_midiNote, MIDI_NOTE_MAX_VOLUME);
      m_midiNote += 2; // ascending whole tone scale
    }
  }

  // Hint counter
  if (m_hints < m_finalHints)
  {
    // TODO && we pass some multiple, e.g. every 100 points
    m_hints++;

    SetHintNumbers();
  }

  // Send a timed message to do this again soon
  // Only if nums are not showing their final values yet
  if (updateAgain)
  {
    // This calculation has the effect of whizzing very quickly though the low numbers, and slowing
    //  down as we reach the 'target' number.
    if (m_topicScore > 0)
    {
      const float NUM_UPDATE_TIME_CONST = .3f;
      float timeToNextUpdate = NUM_UPDATE_TIME_CONST / m_topicScore;
      TheMessageQueue::Instance()->Add(new FuncMsg(::Amju::UpdateNums, SecondsFromNow(timeToNextUpdate)));
    }
  }
}
}


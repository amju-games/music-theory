// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiDecAnimation.h>
#include <Singleton.h>
#include "GSBase.h"
#include "GuiMusicKb.h"
#include "GuiScrollScore.h"
#include "NumUpdate.h"
#include "UserProfile.h"

namespace Amju
{
struct Grade;

// * GSHero *
// Hero mode: the game mechanic which is like Guitar Hero, but for keyboard/sheet music.
class GSHero : public GSBase
{
public:
  GSHero();

  void OnActive() override;
  void Update() override;

  // Called when player triggers a keyboard event
  void OnMusicKbEvent(const MusicKbEvent&) override;

  // Called when Score sends a note event
  void OnNoteEvent(const NoteEvent& ne);

  // Start the score scrolling, and backing track playing
  void Start();

  // Callback, when count-in finishes
  void OnCountInFinished();

protected:
  void InitGui();
 
  // Grade player input event against what we think is the corresponding
  //  event in the score.
  void GradeEvent(const MusicKbEvent& e);

  // Display feedback
  void FeedbackBalloon(const Grade& g);

  void ShowFeedbackBalloon(bool showNotHide);

  // Debug: called when R key is pressed to reload everything.
  void ReloadGui() override;

  void IncreaseScore(const Grade& grade);
  void DecreaseLife(const Grade& grade);

  // Called internally when we detect that player has won or lost round
  void OnPlayerHasLost();
  void OnPlayerHasWon();

protected:
  // The scroll score child of m_gui (get after gui is loaded)
  RCPtr<GuiScrollScore> m_scrollScore;

  // The animator parent of scroll score
  RCPtr<GuiDecAnimation> m_scoreAnim;

  // The keyboard
  RCPtr<GuiMusicKb> m_keyboard;

  // This is for when we grade player input events.
  // This iterator points to the last note event (in m_scrollScore) which
  //  the player attempted, so the player can't spam keys to get lots
  //  of points or make lots of attempts.
  NoteEvents::const_iterator m_prevAttempt;

  // User profile, store progress
  RCPtr<UserProfile> m_userProfile;

  // Score this round - to add to player profile
  HudNumber m_playerScore;

  // Remaining life left this round, as a percentage
  HudNumber m_lifePercent;

  // Counters for detecting missed notes, i.e. no attempt made by
  //  player.
  int m_numPlayerNotes = 0;
  int m_numScoreNotes = 0;

  // Set to true if player wins or loses round, so we can wait for a 
  //  second or two before changing game state.
  bool m_roundIsOver = false;
};

typedef Singleton<GSHero> TheGSHero;
}

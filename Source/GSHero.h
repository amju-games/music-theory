// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiDecAnimation.h>
#include <GuiDecTranslate.h>
#include <Singleton.h>
#include "GSBase.h"
#include "GuiMusicKb.h"
#include "GuiScrollScore.h"
#include "NumUpdate.h"
#include "UserProfile.h"

namespace Amju
{
struct Grade;

// Urgh, better to convert this into State pattern once the
//  structure is clearer.
enum class HeroState
{
  BEFORE_COUNT_IN,
  COUNT_IN,
  SONG_PLAYING,
  ROUND_OVER,
};

// * GSHero *
// Hero mode: the game mechanic which is like Guitar Hero, but for 
//  keyboard/sheet music.
class GSHero : public GSBase
{
public:
  GSHero();

  void OnActive() override;
  void OnDeactive() override;
  void Update() override;
  void Draw2d() override;

  void OnPauseGame() override;

  // Called when player triggers a keyboard event
  void OnMusicKbEvent(const MusicKbEvent&) override;

  // Called when Score sends a note event
  void OnNoteEvent(const NoteEvent& ne);

  // Callback, when count-in finishes - so public.
  void OnCountInFinished();

  // Call this once we know we won't be resuming the current round.
  void CancelResumeTime();

protected:
  // Add an extra GUI element to the score.
  // Specify the event number and type. So you can attach to, say,
  //  the 3rd note on event, or the 2nd rest on event.
  // eventNum is zero-based.
  void AttachExtraBitToScore(PGuiElement extra, int eventNum, NoteEventType net);

  // Attach extra GUI elements to the score -- it would be nice if this
  //  is programmatic rather than specified - could be a mix of both.
  void AttachExtraBits();

  // Called when we restart this state - we may need to resume if we were
  //  paused.
  void ResumeOrRestartGame();

  // Do immediate set up required to resume after a delay
  void SetUpForResume();

  // Restart - if we are not returning from the pause state and so not
  //  resuming
  void RestartGame();

  // Resume having been paused before
  void ResumeGame();

  void InitGui();

  void InitPauseButton();  

  void InitSound();
 
  void SetSongTitle();

  RCPtr<Palette> LoadPalette();

  void InitScrollScore();

  void LoadMusicScore();

  void InitScrollScoreAnim();

  void InitKeyboard();

  void ResetHud();

  void ResetMissedNoteCounters();

  // Look ahead at note events coming up, and translate the keyboard
  //  if necessary, so the keys are on screen.
  void UpdateKeyboardPosition();
 
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
  //RCPtr<GuiDecAnimation> m_scoreAnim;

  // The keyboard
  RCPtr<GuiMusicKb> m_keyboard;

  // Animation controller for keyboard translate
  RCPtr<GuiDecAnimation> m_keyboardAnim;

  // The translation animated by the above anim. This moves
  //  the keyboard so needed notes are on screen.
  RCPtr<GuiDecTranslate> m_keyboardTranslate;

  // A composite on to which we can hang extra stuff we want the score
  //  to display
  RCPtr<GuiComposite> m_scoreExtras;

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

  // If we paused the game while mid-song, this is the time at which
  //  we paused.
  float m_pauseResumeTime = 0;

  // Length of song in music score. 
  float m_scoreLengthSeconds = 0;

  // Time into playing the song. This should be both elapsed time in the
  //  audio track, and the music score.
  float m_animTimeSeconds = 0;

  HeroState m_state = HeroState::BEFORE_COUNT_IN;

  // Value of anim time in previous frame, or zero if we have just
  //  started animating.
  float m_prevAnimTime = 0;
};

typedef Singleton<GSHero> TheGSHero;
}

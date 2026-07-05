// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiDecAnimation.h>
#include <GuiDecTranslate.h>
#include <Singleton.h>
#include "FindSongSections.h"
#include "GSBase.h"
#include "GuiMusicKbBase.h"
#include "GuiPatch.h"
#include "GuiScrollScore.h"
#include "WithHud.h"

namespace Amju
{
struct Grade;
struct HeroGameRound;

// Urgh, better to convert this into State pattern once the
//  structure is clearer.
enum class HeroState
{
  NEW,
  BEFORE_COUNT_IN_RESUME,
  BEFORE_COUNT_IN_RESTART,
  COUNT_IN,
  SONG_PLAYING,
  PLAYER_HAS_WON,
  PLAYER_HAS_LOST,
};

// * GSHero *
// Hero mode: the game mechanic which is like Guitar Hero, but for 
//  keyboard/sheet music.
class GSHero : public GSBase, public WithHud
{
public:
  GSHero();

  void OnActive() override;
  void OnDeactive() override;
  void Update() override;
  void Draw2d() override;

  // Debug: win/lose cheat buttons
  bool OnKeyEvent(const KeyEvent&) override;

  void OnPauseGame() override;

  // Called when player triggers a keyboard event
  void OnMusicKbEvent(const MusicKbEvent&) override;

  // Called when Score sends a note event
  void OnNoteEvent(const NoteEvent& ne);

  // Callback, when count-in finishes - so public.
  void OnCountInFinished();

  // Call this once we know we won't be resuming the current round.
  void CancelResumeTime();

  // Callback, when keyboard stops moving to new position
  void OnKeyboardHasFinishedMoving();

  void SetGameRound(const HeroGameRound* gameRound);
  const HeroGameRound& GetGameRound() const;

protected:
  // Call to change current 'micro state'
  void ChangeState(HeroState newState);

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

  // Find time at which we should resume: the start of the bar, where there
  //  is a note or rest on event.
  void FindResumePoint();

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

  // Debug: called when R key is pressed to reload everything.
  void ReloadGui() override;

  void IncreaseScore(const Grade& grade);
  void DecreaseLife(const Grade& grade);

  // Called internally when we detect that player has won or lost round
  void OnPlayerHasLost();
  void OnPlayerHasWon();

  void ShowCountInGui();

  // Calls the above and starts the count-in song
  void StartCountInSongAndGui();

protected:
  // The scroll score child of m_gui (get after gui is loaded)
  RCPtr<GuiScrollScore> m_scrollScore;

  // The keyboard
  RCPtr<GuiMusicKbBase> m_keyboard;

  // Animation controller for keyboard translate
  RCPtr<GuiDecAnimation> m_keyboardAnim;

  // The translation animated by the above anim. This moves
  //  the keyboard so needed notes are on screen.
  RCPtr<GuiDecTranslate> m_keyboardTranslate;

  // A composite on to which we can hang extra stuff we want the 
  //  musical score to display. E.g. bonus at end of section, etc.
  RCPtr<GuiComposite> m_scoreExtras;

  // Count-in GUI
  PGuiElement m_countInGui;

  // This is for when we grade player input events.
  // This iterator points to the last note event (in m_scrollScore) which
  //  the player attempted, so the player can't spam keys to get lots
  //  of points or make lots of attempts.
  NoteEvents::const_iterator m_prevAttempt;

  // Counters for detecting missed notes, i.e. no attempt made by
  //  player.
  int m_numPlayerNotes = 0;
  int m_numScoreNotes = 0;

  // Set to true if player wins or loses round, so we can wait for a 
  //  second or two before changing game state.
  bool m_roundIsOver = false;

  // If we paused the game while mid-song, this is the normalised
  //  time at which we paused.
  float m_pauseResumeTime = 0;

  // Length of song in music score in seconds -- NOT normalised, which 
  //  would be 1! 
  float m_scoreLengthSeconds = 0;

  // Current 'micro state'... upgrade to State pattern?
  HeroState m_state = HeroState::NEW;

  // Value of anim time in previous frame, or zero if we have just
  //  started animating.
  float m_prevAnimTime = 0;

  // Time (seconds) we have been in the current Hero State
  float m_timeInHeroState = 0;

  // Duration of count-in track; after this time, change state to SONG_PLAYING.
  float m_countInExpiryTime = 0;

  // Sections of the song: we update the keyboard position and colours
  //  when entering a new section, (not continuously/every note event).
  Sections m_songSections;
  int m_sectionIndex = 0; // index into m_songSections

  // While true, keyboard is moving to a new position
  bool m_keyboardIsMoving = false;
};

typedef Singleton<GSHero> TheGSHero;
}

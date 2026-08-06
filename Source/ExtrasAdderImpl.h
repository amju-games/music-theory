#pragma once

#include <GuiElement.h>
#include "Extra.h"
#include "FindSongSections.h"

namespace Amju
{
class GuiMusicScore;

// * Extras Adder Impl *
// Adds Extras to the score. 
class ExtrasAdderImpl 
{
public:
  ExtrasAdderImpl(
    PGuiElement extrasRoot,
    const GuiMusicScore& musicScore,
    const Sections& songSections) : 
    m_extrasRoot(extrasRoot),
    m_musicScore(musicScore),
    m_songSections(songSections) {}

  // Programatically add extras to the given music score by adding
  //  children to the given GUI root.
  // But we should only add Extras after the given NoteEvent ID,
  //  because we resume from paused, and don't want to give extras
  //  for the part of the song already played!
  // Could be another ctor param, as we will store it as a member.
  // Hmm, we need to keep track of multi-note extras that are partially
  //  complete?! That's a pain! There should only be one of those 
  //  in flight at a time tho, so we only need to store the most 
  //  recent/current extra.
  void AttachExtraBits(int fromThisNoteId);

  // Correct note: collect the extra at the given event.
  // Move the extra from the scrolling root to the given non-scrolling
  //  root (could be ctor param instead).
  void CollectExtra(int eventId, GuiComposite* nonScrollingRoot);

  // Bum note or missed note: if there is an extra attached to the
  //  given event, show it has been missed.
  // For a multi-event extra, (e.g. note run), the remaining child extras
  //  and the final payoff are all lost, right? Bad one.
  void NoCollectExtra(int eventId);

protected:
  // Returns true if an extra has already been allocated to the
  //  given event ID.
  bool IsExtraAllocated(int eventId) const;

  void AddSectionExtras(GuiComposite* extrasRootComp);

  void AddNoteRunExtras();

  // Sprinkles extras in the song for note events that have not already
  //  been allocated an extra.
  void AddRandomExtras(
    GuiComposite* extrasRootComp,
    const NoteEvents& noteEvents,
    int fromThisNoteId);

  // Attach the given Extra to the scrolling score.
  // eventId is note event ID, unique in the song, so we can retrieve the
  //  extra later when collected or not.
  void AttachExtraBitToScore(
    GuiComposite* scrollingRoot,
    int eventId,  // 'global' ID into all note events, not just note on events.
    IExtra* extra);

  // Attach a points multiplier extra to the given gui root, for the given
  //  note on event.
  void AttachPointsMultiplier(GuiComposite* extrasRootComp, int noteEventId);

  // Attach a health boost extra to the given GUI root for the given event.
  void AttachHealthBoost(GuiComposite* extrasRootComp, int noteEventId);

protected:
  PGuiElement m_extrasRoot;
  const GuiMusicScore& m_musicScore;
  const Sections& m_songSections;
  int m_minEventId = 0; // we only add from this event ID onwards.

  using NoteIdToExtra = std::unordered_map<int, PExtra>;
  NoteIdToExtra m_extrasMap;
};
}


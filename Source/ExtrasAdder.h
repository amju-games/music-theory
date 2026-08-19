#pragma once

#include <GuiElement.h>
#include "FindSongSections.h"

namespace Amju
{
class ExtrasAdderImpl;
class GuiComposite;
class GuiMusicScore;

// * Extras Adder *
// Adds Extras to the score. 
class ExtrasAdder : public RefCounted
{
public:
  ExtrasAdder(
    PGuiElement extrasRoot,
    const GuiMusicScore& musicScore,
    const Sections& songSections);

  virtual ~ExtrasAdder();

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
  ExtrasAdderImpl* m_pimpl;
};
}


#include <AmjuRand.h>
#include <GuiComposite.h>
#include "ExtrasAdder.h"
#include "ExtrasAdderImpl.h"

namespace Amju
{
ExtrasAdder::ExtrasAdder(
  PGuiElement extrasRoot,
  const GuiMusicScore& musicScore,
  const Sections& songSections) : 
  m_pimpl(new ExtrasAdderImpl(extrasRoot, musicScore, songSections))
{
}

ExtrasAdder::~ExtrasAdder()
{
  delete m_pimpl;
}

void ExtrasAdder::NoCollectExtra(int eventId)
{
  m_pimpl->NoCollectExtra(eventId);
}

void ExtrasAdder::CollectExtra(int eventId, GuiComposite* nonScrollingRoot)
{
  m_pimpl->CollectExtra(eventId, nonScrollingRoot);
}

void ExtrasAdder::AttachExtraBits(int fromThisNoteId) 
{
  m_pimpl->AttachExtraBits(fromThisNoteId);
}
}


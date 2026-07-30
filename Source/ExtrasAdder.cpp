#include <GuiComposite.h>
#include "ExtrasAdder.h"
#include "GuiMusicScore.h"

namespace Amju
{
void ExtrasAdder::CollectExtra(int eventId, GuiComposite* nonScrollingRoot)
{
  // Assume this gets called for every correct event: 
  //  do nothing if no extra for the event.
  auto it = m_extrasMap.find(eventId);
  if (it == m_extrasMap.end()) 
  {
    return; // No extra for this event.
  }

  std::cout << "Collecting extra for note event " << eventId << "\n";

  auto extra = it->second;

  GuiComposite* extrasRootComp = dynamic_cast<GuiComposite*>(m_extrasRoot.GetPtr());

  // Start collecting the extra: detach from scrolling score, attach to
  //  non-scrolling score, and start path animation.
  extra->StartCollection(extrasRootComp, nonScrollingRoot);
}

void ExtrasAdder::AttachExtraBitToScore(
  GuiComposite* extrasRootComp,
  int eventId, 
  IExtra* extra) 
{
  // Add the extra we create to map: so we don't add more than
  //  one extra to any one event; and we can look up the extra to 
  //  collect it when event is graded as correct.
  Assert(m_extrasMap.find(eventId) == m_extrasMap.end());
  m_extrasMap[eventId] = extra;

std::cout << "Extras: adding extra for event: " << eventId << "\n";

  // Get pos on score where we should attach Extra gui
  const auto& ne = m_musicScore.GetNoteEvents()[eventId];
  Vec2f pos = ne.GetPos();
  pos *= m_musicScore.GetSize(); // scale to match size of score

  // Attach extra to scrolling music score
  extra->AttachToScrollingRoot(extrasRootComp, pos);
}

void ExtrasAdder::AttachExtraBits(
  int fromThisNoteId) 
{
  // Get rid of all events except for the type we are looking for.
  // For now, we are only supporting note on events.
  auto noteEvents(m_musicScore.GetNoteEvents()); 
  const auto net = NoteEventType::NOTE_ON;
  noteEvents.erase(
    std::remove_if(noteEvents.begin(), noteEvents.end(),
     [=](const NoteEvent& ne) { return ne.m_type != net; }),
    noteEvents.end());

  GuiComposite* extrasRootComp = dynamic_cast<GuiComposite*>(m_extrasRoot.GetPtr());
  AddSectionExtras();

  AddNoteRunExtras(); // needs note on events only, right?

  AddRandomExtras(extrasRootComp, noteEvents, fromThisNoteId);
}

void ExtrasAdder::AddSectionExtras()
{
}

void ExtrasAdder::AddNoteRunExtras()
{
  // For a given run, add MultiChild extras to the events, wrapped in 
  //  an owning MultiEventExtra.
}

void ExtrasAdder::AddRandomExtras(
  GuiComposite* extrasRootComp,
  const NoteEvents& noteEvents,
  int fromThisNoteId)
{
  // TODO
  // Start with vec of all event ids.
  // Remove event ids in m_extrasMap.
  // Shuffle remaining vec.
  // Allocate extras to the first <n> ids in vec.

  // TODO TEMP TEST:
  // Attach a heart to the score.
  // This is a standard Extra type.
  for (int i = 0; i < 10; i++)
  {
    auto gui = LoadGui("Gui/extra-heart.txt");
    auto reward = new RewardHealth(10); 
    auto extra = new Extra(gui, reward);
 
    AttachExtraBitToScore(extrasRootComp, noteEvents[i].m_id, extra);
  }
}
}


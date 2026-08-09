#include <AmjuRand.h>
#include <GuiComposite.h>
#include <GuiText.h>
#include "ExtrasAdderImpl.h"
#include "GuiMusicScore.h"
#include "MultiExtra.h"
#include "NoteRun.h"

#define EXTRA_DEBUG

namespace Amju
{
// Get random value for points mutiplier reward
static int GetRandomPointsMultiplier()
{
  // Let's say 80% chance of x2, 20% chance of x5.
  // As these accumulate we don't want to go crazy.
  return (Rnd(0.f, 1.f) > .8f ? 5 : 2);
}

// Get random value for health boost reward
static int GetRandomHealthBoost()
{
  // 60% chance of +2, 30% chance of +5, 10% chance of +10.
  const float r = Rnd(0.f, 1.f);
  return (r < .6f ? 2 : (r < .9 ? 5 : 10));
}

// Set the text node named "extra-text" with the given string.
static void SetRewardGuiText(PGuiElement gui, const std::string& text)
{
  auto elem = GetElementByName(gui, "extra-text");
  Assert(elem);
  auto textNode = dynamic_cast<GuiTextBase*>(elem);
  Assert(elem);
  textNode->SetText(text);
}

bool ExtrasAdderImpl::IsExtraAllocated(int eventId) const
{
  return m_extrasMap.contains(eventId);
}

void ExtrasAdderImpl::NoCollectExtra(int eventId)
{
  // This gets called for every event: do nothing if no extra for the event.
  auto it = m_extrasMap.find(eventId);
  if (it == m_extrasMap.end()) 
  {
#ifdef EXTRA_DEBUG
std::cout << "No collect: this ID does not have an extra??: " << eventId << "\n";
#endif
    return; // No extra for this event.
  }

#ifdef EXTRA_DEBUG
  std::cout << "NOT Collecting extra for note event " << eventId << "\n";
#endif

  auto extra = it->second;

  // Start 'not collected' anim - no need to add to non-scrolling root,
  //  we can continue scrolling, but we will detach from scrolling root
  //  when anim is finished.
  extra->StartNoCollect();
}

void ExtrasAdderImpl::CollectExtra(int eventId, GuiComposite* nonScrollingRoot)
{
  // Assume this gets called for every correct event: 
  //  do nothing if no extra for the event.
  auto it = m_extrasMap.find(eventId);
  if (it == m_extrasMap.end()) 
  {
    return; // No extra for this event.
  }

#ifdef EXTRA_DEBUG
  std::cout << "Collecting extra for note event " << eventId << "\n";
#endif

  auto extra = it->second;

  GuiComposite* extrasRootComp = dynamic_cast<GuiComposite*>(m_extrasRoot.GetPtr());

  // Start collecting the extra: detach from scrolling score, attach to
  //  non-scrolling score, and start path animation.
  extra->StartCollection(extrasRootComp, nonScrollingRoot);
}

void ExtrasAdderImpl::AttachExtraBitToScore(
  GuiComposite* extrasRootComp,
  int eventId, 
  IExtra* extra) 
{
#ifdef EXTRA_DEBUG
std::cout << "Extras at these IDs: ";
for (const auto& [id, extra] : m_extrasMap)
{
  std::cout << id << " ";
}
std::cout << ", adding " << eventId << "\n";
#endif

  // Add the extra we create to map: so we don't add more than
  //  one extra to any one event; and we can look up the extra to 
  //  collect it when event is graded as correct.
  Assert(m_extrasMap.find(eventId) == m_extrasMap.end());
  m_extrasMap[eventId] = extra;

#ifdef EXTRA_DEBUG
std::cout << "Extras: adding extra for event: " << eventId << "\n";
#endif

  // Get pos on score where we should attach Extra gui
  const auto& ne = m_musicScore.GetNoteEvents()[eventId];
  Vec2f pos = ne.GetPos();
  pos *= m_musicScore.GetSize(); // scale to match size of score

  // Attach extra to scrolling music score
  extra->AttachToScrollingRoot(extrasRootComp, pos);
}

void ExtrasAdderImpl::AttachExtraBits(int fromThisNoteId) 
{
  // Get rid of all events except for the type we are looking for.
  // For now, we are only supporting note on events.
  auto noteEvents(m_musicScore.GetNoteEvents()); 
  const auto net = NoteEventType::NOTE_ON;
  noteEvents.erase(
    std::remove_if(noteEvents.begin(), noteEvents.end(),
     [=](const NoteEvent& ne) { return ne.m_type != net; }),
    noteEvents.end());
  // NB noteEvents are Note ON events only!

  auto extrasRootComp = dynamic_cast<GuiComposite*>(m_extrasRoot.GetPtr());

  AddSectionExtras(extrasRootComp);

  AddNoteRunExtras(extrasRootComp, noteEvents); 

  AddRandomExtras(extrasRootComp, noteEvents, fromThisNoteId);
}

void ExtrasAdderImpl::AttachHealthBoost(
  GuiComposite* extrasRootComp, int noteEventId)
{
  // Load gui
  auto gui = LoadGui("Gui/extra-heart.txt");

  // Get boost value, e.g. 2, 5, 10
  const int healthBoost = GetRandomHealthBoost();

  // Create reward
  auto reward = new RewardHealth(healthBoost); 

  // Set text in GUI
  SetRewardGuiText(gui, "+" + std::to_string(healthBoost));

  // Create and add Extra.
  // This is a standard Extra type.
  auto extra = new Extra(gui, reward);
  AttachExtraBitToScore(extrasRootComp, noteEventId, extra);
}

void ExtrasAdderImpl::AttachPointsMultiplier(
  GuiComposite* extrasRootComp, int noteEventId)
{
  // Load points multiplier gui
  auto gui = LoadGui("Gui/extra-points-mult.txt");

  // Multiplier, e.g. 2, 5
  int pointsMult = GetRandomPointsMultiplier(); 

  // Create Reward for this extra
  auto reward = new RewardPointsMult(pointsMult);

  // Set text in GUI 
  SetRewardGuiText(gui, "x" + std::to_string(pointsMult));

  // Create extra, add to scrolling root.
  // This is a standard Extra type, not a multi-extra.
  auto extra = new Extra(gui, reward);
  AttachExtraBitToScore(extrasRootComp, noteEventId, extra);
}

MultiExtra* ExtrasAdderImpl::AttachMultiPoints(
  GuiComposite* extrasRootComp, int noteEventId, int points)
{
std::cout << " .. attaching multi points extra to event " << noteEventId 
  << " (" << points << " points)\n";

  // Load points add gui
  auto gui = LoadGui("Gui/extra-points.txt");
  Assert(gui);

  // Create Reward for this extra
  auto reward = new RewardPoints(points);

  // Set text in GUI 
  SetRewardGuiText(gui, "+" + std::to_string(points));

  // Create extra, add to scrolling root.
  auto extra = new MultiExtra(gui, reward);
  AttachExtraBitToScore(extrasRootComp, noteEventId, extra);

  return extra;
}

void ExtrasAdderImpl::AttachChildPoints(
  GuiComposite* extrasRootComp, int noteEventId, int points,
  MultiExtra* multiParent)
{
std::cout << " .. attaching child points extra to event " << noteEventId
  << " (" << points << " points)\n";
 
  // Load points add gui
  auto gui = LoadGui("Gui/extra-points.txt");

  // Create Reward for this extra. Destination will be the final Extra.
  auto reward = new RewardPointsChild(points, multiParent->GetGui());

  // Set text in GUI 
  SetRewardGuiText(gui, "+" + std::to_string(points));

  // Create extra, add to scrolling root.
  auto extra = new ChildExtra(gui, reward, multiParent);
  AttachExtraBitToScore(extrasRootComp, noteEventId, extra);
}

void ExtrasAdderImpl::AddSectionExtras(GuiComposite* extrasRootComp)
{
  const auto& noteEvents = m_musicScore.GetNoteEvents();
  int sectionNum = 0;
  for (auto [first, last] : m_songSections)
  {
    // Iterate back from last to find the last note-on event
    while (last > first)
    {
      --last;
      const auto& ne = noteEvents[last];
      const int id = ne.GetId();
      if (ne.IsNoteOnEvent())
      {
        // Found final note-on event in the section
#ifdef EXTRA_DEBUG
std::cout << "Extras: adding extra to final note on event in section " 
  << sectionNum
  << " Note event ID: " << id
  << "\n";
#endif
        ++sectionNum;
        AttachPointsMultiplier(extrasRootComp, id);
        break;
      }
    }
  }
}

void ExtrasAdderImpl::AddNoteRunExtras(
  GuiComposite* extrasRootComp, 
  const NoteEvents& noteOnEvents)
{
  // For a given run, add MultiChild extras to the events, wrapped in 
  //  an owning MultiEventExtra.

  // Find note runs - partial scales in the song. 
  const int MIN_DIFF = 1;
  const int MAX_DIFF = 3;
  auto runs = FindNoteRunsNoNoteOffEvents(noteOnEvents, MIN_DIFF, MAX_DIFF);

  // Sort the runs by length
  std::sort(runs.begin(), runs.end(), 
    [](const NoteRun& nr1, const NoteRun& nr2)
    {
      // descending order of size
      return nr1.m_ids.size() > nr2.m_ids.size();
    }
  );

#ifdef NOTE_RUN_DEBUG
std::cout << "SORTED NOTE RUNS:\n";
  PrintRuns(runs, m_musicScore.GetNoteEvents());
#endif

  int numNoteRunExtras = runs.size() / 3; // TODO TEMP TEST
  for (int i = 0; i < numNoteRunExtras; ++i)
  {
    const auto& run = runs[i];
    AddExtrasForOneRun(extrasRootComp, run);
  }
}

void ExtrasAdderImpl::AddExtrasForOneRun(
  GuiComposite* extrasRootComp, const NoteRun& run)
{
  const int numNotesInRun = run.m_ids.size();

  // Check for overlap with existing Extra
  for (int note = 0; note < numNotesInRun; ++note)
  {
    int id = run.m_ids[note];
    if (IsExtraAllocated(id)) return;
  }

std::cout << "Adding extras for note run: ";
for (int note = 0; note < numNotesInRun; ++note) 
{ 
  std::cout << run.m_ids[note] << " "; 
}
std::cout << "\n";

  // Create MultiExtra for the last note
  int sumOfPoints = 5000; // TODO TEMP TEST
  auto multiExtra = AttachMultiPoints(extrasRootComp, run.m_ids.back(), sumOfPoints);

  // Create ChildExtras for all the notes in the run, up to but not including
  //  the final note.
  int points = 100;
  for (int note = 0; note < numNotesInRun - 1; ++note)
  { 
    int id = run.m_ids[note];
    
    // Create ChildExtra
    AttachChildPoints(extrasRootComp, id, points, multiExtra);

    points *= 2; // Or have an array of values we index into, with a max.
  }
}

void ExtrasAdderImpl::AddRandomExtras(
  GuiComposite* extrasRootComp,
  const NoteEvents& noteOnEvents,
  int fromThisNoteId)
{
  // Start with vec of all NOTE ON event ids, WITHOUT the event ids 
  //  to which we have already allocated an Extra - i.e. those in m_extrasMap.
  // Shuffle the vec of remaining IDs.
  // Allocate extras to the first <n> ids in vec.
  std::vector<int> eventIds;
  eventIds.reserve(noteOnEvents.size());
  // Add the note event IDs that are NOT in m_extrasMap.
  std::for_each(noteOnEvents.begin(), noteOnEvents.end(), 
    [&eventIds, this](const NoteEvent& ne) 
    {
      if (!m_extrasMap.contains(ne.GetId())) { eventIds.push_back(ne.GetId()); }
    }
  );

#ifdef RANDOM_EXTRAS_DEBUG
std::cout << "Vec of unallocated IDs.....\n";
for (int i : eventIds) { std::cout << i << " "; }
std::cout << "\n";
#endif

  Amju::RandomShuffle(eventIds.begin(), eventIds.end(), Amju::RandomInt);

#ifdef RANDOM_EXTRAS_DEBUG
std::cout << "SHUFFLED IDs.....\n";
for (int i : eventIds) { std::cout << i << " "; }
std::cout << "\n";
#endif

  // Number of extras we add here should be a proportion of the total number
  //  of notes. 
  int numExtras = eventIds.size() / 10; // TODO config? Set in .csv??
  // Make sure we don't overrun!
  numExtras = std::min(numExtras, static_cast<int>(eventIds.size()));

#ifdef RANDOM_EXTRAS_DEBUG
std::cout << "Allocating random extra to " << numExtras << " extras.\n";
#endif

  for (int i = 0; i < numExtras; i++)
  {
    const int id = eventIds[i];

    Assert(!IsExtraAllocated(id));

#ifdef RANDOM_EXTRAS_DEBUG
std::cout << "Allocating an extra to event: " << id << "\n";
#endif

    // TODO More Extra types.
    AttachHealthBoost(extrasRootComp, id);
  }
}
}


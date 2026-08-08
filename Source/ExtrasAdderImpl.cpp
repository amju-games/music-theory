#include <AmjuRand.h>
#include <GuiComposite.h>
#include <GuiText.h>
#include "ExtrasAdderImpl.h"
#include "GuiMusicScore.h"

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
  // Assume this gets called for every correct event: 
  //  do nothing if no extra for the event.
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

  AddNoteRunExtras(); // TODO needs note on events only, right?

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

void ExtrasAdderImpl::AddNoteRunExtras()
{
  // For a given run, add MultiChild extras to the events, wrapped in 
  //  an owning MultiEventExtra.
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

std::cout << "Vec of unallocated IDs.....\n";
for (int i : eventIds) { std::cout << i << " "; }
std::cout << "\n";

  Amju::RandomShuffle(eventIds.begin(), eventIds.end(), Amju::RandomInt);

std::cout << "SHUFFLED IDs.....\n";
for (int i : eventIds) { std::cout << i << " "; }
std::cout << "\n";

  // Number of extras we add here should be a proportion of the total number
  //  of notes. 
  int numExtras = eventIds.size() / 10; // TODO config? Set in .csv??
  // Make sure we don't overrun!
  numExtras = std::min(numExtras, static_cast<int>(eventIds.size()));

std::cout << "Allocating random extra to " << numExtras << " extras.\n";

  for (int i = 0; i < numExtras; i++)
  {
    const int id = eventIds[i];

    Assert(!IsExtraAllocated(id));

    // TODO More Extra types.
std::cout << "Allocating an extra to event: " << id << "\n";

    AttachHealthBoost(extrasRootComp, id);
  }
}
}


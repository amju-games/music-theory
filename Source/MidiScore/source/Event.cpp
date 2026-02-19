// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include "Event.h"
#include "MidiScore.h" // OutputEvents
#include "TimeSig.h"

namespace MidiScore
{
void AppendNoteEventToEvents(Event e, Events& events)
{
  events.emplace_back(e);
}

static int Signum(int s) // sigh, no standard func?
{
  return s > 0 ? 1 : (s < 0 ? -1 : 0); 
}

void InsertDynamics(Events& events)
{
  for (int i = 0; i < events.size(); i++)
  {
    Event& e = events[i];
    if (i == 0)
    {
      e.m_dynamics.SetMark(); // always on note 0
    } 
    else
    {
      int change1 = e.m_dynamics.m_velocity - events[i - 1].m_dynamics.m_velocity;
      if (i > 1)
      {
        int change2 = events[i - 1].m_dynamics.m_velocity - 
          events[i - 2].m_dynamics.m_velocity;

        if (Signum(change1) != Signum(change2))
        {
          // Vec direction has changed - add a mark to the prev note
          events[i - 1].m_dynamics.SetMark();
        }

        // Final note
        if (i == events.size() - 1 && change1 != 0)
        {
          e.m_dynamics.SetMark();
        }
      }
    }
  }
}

TimeVal GetTimeValFromString(const std::string& s)
{
  static const std::map<std::string, TimeVal> TVS =
  {
    { "qqq", TimeVal::QQQ },
    { "qq", TimeVal::SEMIQUAVER },
    { "q", TimeVal::QUAVER },
    { "c", TimeVal::CROTCHET },
    { "m", TimeVal::MINIM },
    { "sb", TimeVal::SEMIBREVE },
    { "sb2", TimeVal::SB2 },
    { "sb4", TimeVal::SB4 },
  };
  return TVS.at(s);
}

std::string TimeValString(TimeVal t)
{
  if (t == TimeVal::QQQ) return "qqq";
  if (t == TimeVal::SEMIQUAVER) return "qq";
  if (t == TimeVal::QUAVER) return "q";
  if (t == TimeVal::CROTCHET) return "c";
  if (t == TimeVal::MINIM) return "m";
  if (t == TimeVal::SEMIBREVE) return "sb";
  if (t == TimeVal::SB2) return "sb2";
  if (t == TimeVal::SB4) return "sb4";
  return "BAD TIME VAL";
}

std::string Event::NoteToStringNoDuration() const
{
  assert(m_type == EventType::NOTE);

  // Dynamics refer to the last note, so pitch comes first.
  auto res = std::to_string(m_pitch);
  auto d = m_dynamics.ToString();
  if (!d.empty())
  {
    res += " " + d;
  }
  // TODO also articulation
  return res;
}

std::string Event::DurationString() const
{
  return TimeValString(m_timeVal) + 
    (m_dots > 0 ? std::string(m_dots, '.') : ""); 
}

std::string Event::ToString() const
{
  switch (m_type)
  {
  case EventType::REST:
    return "<" + DurationString() + "> r";
  
  case EventType::NOTE: 
    // Duration is in angle brackets, which means it applies
    //  to all subsequent notes.
    return "<" + DurationString() + "> " + NoteToStringNoDuration();

  case EventType::BARLINE:
    return "|";

  case EventType::TIE:
    return "t";

  case EventType::CHORD_START: 
    return "(";
 
  case EventType::CHORD_END: 
    return ")";

  case EventType::TIME_SET:
    {
      std::stringstream ss;
      ss << "time " << m_timeSetVal;
      return ss.str();
    }

  default:
    std::cout << "No String for Event Type " 
      << static_cast<int>(m_type) << "\n";
    assert(0);
    break;
  }
  return "";
}

// Set time values in an Event, given m_duration member and tpq. 
void Event::SetTimeVal(int tpq)
{
  // Set up sequence of exact multiples of tpq. We expect the raw event
  //  duration to be one of these multiples, but it could be off. So
  //  get the closest multiple, set the TimeVal, and then correct duration 
  //  and end times to the 'perfect', exact values. This will avoid 
  //  inserting crazy rests between note events with funny durations.
  //
  // Each element here is tpq multiple, time val, and number of dots.
  // (Could insert more elements to support double dots.)
  // Not static, tpq can be different each time!
  const std::vector<std::tuple<int, TimeVal, int>> MULTIPLES = 
  { 
    { tpq / 8,      TimeVal::QQQ,         0 }, // qqq
    { 3 * tpq / 16, TimeVal::QQQ,         1 }, // qqq.
    { tpq / 4,      TimeVal::SEMIQUAVER,  0 }, // qq
    { 3 * tpq / 8,  TimeVal::SEMIQUAVER,  1 }, // qq.
    { tpq / 2,      TimeVal::QUAVER,      0 }, // q
    { 3 * tpq / 4,  TimeVal::QUAVER,      1 }, // q.
    { tpq,          TimeVal::CROTCHET,    0 }, // c
    { 3 * tpq / 2,  TimeVal::CROTCHET,    1 }, // c.
    { 2 * tpq,      TimeVal::MINIM,       0 }, // m.
    { 3 * tpq,      TimeVal::MINIM,       1 }, // m.
    { 4 * tpq,      TimeVal::SEMIBREVE,   0 }, // sb
    { 6 * tpq,      TimeVal::SEMIBREVE,   1 }, // sb.
    { 8 * tpq,      TimeVal::SB2,         0 }, // sb2
    { 12 * tpq,     TimeVal::SB2,         1 }, // sb2.
    { 16 * tpq,     TimeVal::SB4,         0 }, // sb4
    { 24 * tpq,     TimeVal::SB4,         1 }, // sb4.
  };

  // Get first element >= m_duration
  auto it = std::lower_bound(MULTIPLES.begin(), MULTIPLES.end(), m_duration, 
    [] (const auto& m, int dur) { return std::get<0>(m) < dur; }
  );
  
  if (it == MULTIPLES.end())
  {
    // Duration is bigger than sb4. (with the dot).
    --it; // use largest supported value, (but it's wrong).
    // TODO We need a better way of reporting errors
    std::cout << "// *** Event duration too large! Event pitch: " 
      << m_pitch 
      << " at time: " 
      << m_start 
      << " (" 
      << m_start/tpq 
      << " crotchets), duration: "
      << m_duration 
      << " ticks.\n"; 
  } 
  else if (it == MULTIPLES.begin()) 
  {
    // Is duration smaller than smallest type?
    if (m_duration < std::get<0>(MULTIPLES.front()))
    {
      std::cout << "// *** Event duration too small! Event pitch: " 
        << m_pitch 
        << " at time: " 
        << m_start 
        << " (" 
        << m_start/tpq 
        << " crotchets), duration: "
        << m_duration 
        << " ticks.\n"; 
    }
  }
  else
  {
    // Choose between current or the previous element, whichever has closest
    //  duration to m_duration.
    // (Element *it is >= duration, but maybe the previous element, which is
    //  < duration, is closer to it.)
    const int iDuration = std::get<0>(*it);
    auto jt = it - 1;
    const int jDuration = std::get<0>(*jt);
    assert(iDuration >= m_duration);
    assert(m_duration >= jDuration);
    if ((iDuration - m_duration) > (m_duration - jDuration))
    {
      it = jt; // prev element is closer to m_duration
    }
  }

  // Set members.
  const auto [duration, timeVal, dots] = *it;
  m_timeVal = timeVal;
  m_duration = duration;
  m_dots = dots;
  m_end = m_start + m_duration; // recalc this
}

void Event::QuantiseDuration(int tpq, TimeVal resolution)
{
  assert(m_unquantisedDuration > -1);

  const std::array<int, 8> MULTS = 
  {{
    tpq/8, tpq/4, tpq/2, tpq, tpq*2, tpq*4, tpq*8, tpq*16,
  }};
  int mult = MULTS[static_cast<int>(resolution)];

  m_duration = mult * std::max(1.f,  // duration is at least resolution
    std::round(
      static_cast<float>(m_unquantisedDuration) / 
      static_cast<float>(mult)));
 
  SetTimeVal(tpq); 
}

void Event::QuantiseStartTime(int tpq, TimeVal resolution)
{
  // If -1, unquantised start time was not set when
  //  event was created.
  assert(m_unquantisedStart > -1);

  const std::array<int, 8> MULTS = 
  {{
    tpq/8, tpq/4, tpq/2, tpq, tpq*2, tpq*4, tpq*8, tpq*16,
  }};

  // Look up the mult value for the given resolution. This is a straight
  //  lookup - it's not a search for closest.
  int mult = MULTS[static_cast<int>(resolution)];

  // Get closest multiple of mult to the unquantised start time.
  m_start = mult * std::round(
    static_cast<float>(m_unquantisedStart) / 
    static_cast<float>(mult));
}

Event MakeRest(int tpq, int duration, int start)
{
  Event rest;
  rest.m_duration = duration;
  rest.m_start = start;
  rest.m_end = start + duration;
  rest.SetTimeVal(tpq);
  rest.m_type = EventType::REST;

  return rest;
}

void InsertRests(int tpq, Events& events)
{
  // To insert rests, we get the end time of each event, and compare it
  //  with the start time of the next event. If there is a gap, we should
  //  insert a rest there.
  // *That's ok except for within a chord, because the notes in the chord
  //  could have different durations. We don't want to insert a rest after 
  //  the shorter note, because it could clash with another note being
  //  played at the same time as the rest.

  int t = 0;  // accumulated time ticks through the piece
  bool chord = false; // true if we are parsing between ( ) chord markers

  for (auto it = events.begin(); it != events.end(); ++it)
  {
    if (it->IsChordStart()) chord = true;
  
    if (it->m_start > t)
    {
      // Gap found between accumulated time so far, and the start time of
      //  the current event. So we should insert a rest here.
      if (!chord) // Except, no rests within chord markers. See *.
      {
        int restDuration = it->m_start - t;
        it = events.insert(it, MakeRest(tpq, restDuration, t));
        ++it;
      }
    }
    t = it->m_end;

    // Reset flag here, after testing for being in a chord.
    // We don't want to add rests within a chord, but if we did, it would
    //  be right before the chord end marker.
    if (it->IsChordEnd()) chord = false;
  }
}

Event MakeBarLine(int startTicks)
{
  Event e;
  e.m_type = EventType::BARLINE;
  e.m_start = startTicks;
  e.m_duration = 0;
  e.m_end = startTicks;
  return e;
}

Event MakeTie(int startTicks)
{
  Event e;
  e.m_type = EventType::TIE;
  e.m_start = startTicks;
  e.m_duration = 0;
  e.m_end = startTicks;
  return e;
}

Event MakeChordStart(int startTicks)
{
  Event e;
  e.m_type = EventType::CHORD_START;
  e.m_start = startTicks;
  e.m_duration = 0;
  e.m_end = startTicks;
  return e;
}

Event MakeChordEnd(int startTicks)
{
  Event e;
  e.m_type = EventType::CHORD_END;
  e.m_start = startTicks;
  e.m_duration = 0;
  e.m_end = startTicks;
  return e;
}

// Split chord across bar lines, return number of bar lines created.
int SplitChord(int tpq, Events& events, Events::iterator& it, int barLineTicks,
  int ticksForOneBar)
{
  // E.g., something like this:
  //  ( <m> 60 61 ) -> ( <c> 60 61 ) | t ( <c>  60 61 )
  // So MakeScore can see there are chords before and after the bar line.

  // When we start, we have skipped over the CHORD_START event, i.e.
  //  in the above example, we are pointing to the first note in the chord:
  //
  //  60 61 ) 
  //  ^
  //  it

  int bar = 0;
  while (it->m_start < barLineTicks && it->m_end > barLineTicks)
  {
    // Get the notes in the chord.. find the chord end marker
    auto chordEnd = it;
    while (chordEnd != events.end() && !chordEnd->IsChordEnd()) ++chordEnd;
     
    // Make a vec of the 'surviving' notes the the chord, i.e. they
    //  have a duration beyond the duration until the bar line. 
    Events notes;
    // Cut the duration of all the notes in the chord, to the bar line
    for (auto jt = it; jt != chordEnd; ++jt)
    {
      Event note(*jt); // copy of the note, for the next bar.

      // urgh, maybe some notes are shorter and don't get split, or
      //  don't even reach the bar line.
      jt->m_end = std::min(jt->m_end, barLineTicks); 

      // Set new duration and time val
      jt->m_duration = jt->m_end - jt->m_start;
      jt->SetTimeVal(tpq);

      // Create remaining portion of this note
      note.m_start = barLineTicks;
      note.m_duration = note.m_end - note.m_start;
      // If any remainder, add to chord to add to the next bar.
      if (note.m_duration > 0)
      {
        note.SetTimeVal(tpq);
        notes.push_back(note);
      }
    }
    it = chordEnd;
    // Situation now:
    //  60 61 ) 
    //        ^
    //        it
    ++it;
    it = events.insert(it, MakeBarLine(barLineTicks));
    // Add tie if any notes 'survive' to the next bar: at least one should do.
    if (!notes.empty())
    {
      ++it;
      it = events.insert(it, MakeTie(barLineTicks));
    }
    // Now:
    //  60 61 ) | t
    //            ^
    //            it
   
    // Add the notes and chord end marker, (which has time of note end time)
    if (notes.size() > 1) // if only one note surviving, it's not a chord.
    {
      // Add the chord END marker to the end of the notes in the chord.
      notes.push_back(MakeChordEnd(notes.front().m_end));
      // Insert the chort START marker into the event list.
      ++it;
      it = events.insert(it, MakeChordStart(barLineTicks));
    }
    // Insert the notes in the chord + end marker into the event list.
    ++it;
    it = events.insert(it, notes.begin(), notes.end());
    // it points to first note added, nice for the next iteration.
     
    bar++;
    barLineTicks += ticksForOneBar; 
    // loop and chop second notes as before if required
  }
  return bar;
}

// Split note across bar lines, return number of bar lines created.
int SplitNote(int tpq, Events& events, Events::iterator& it, int barLineTicks,
  int ticksForOneBar)
{
  // One note -> first part of note, bar line, tie, final part of note.
  // If the note value is greater than a bar length, we repeat the
  //  splitting process.
  // Before:
  //  it -> <note>
  // After:
  //        <first note>
  //        <barline>
  //        <tie>
  //  it -> <second note>

  int bar = 0;
  while (it->m_start < barLineTicks && it->m_end > barLineTicks)
  {
    Event& firstNote = *it;
    Event secondNote(*it); // copy of first note, to go in next bar

    // Cut duration of first note to bar line
    firstNote.m_end = barLineTicks;
    firstNote.m_duration = firstNote.m_end - firstNote.m_start;
    firstNote.SetTimeVal(tpq);

    // Second note is the duration difference, which can overrun the 
    //  length of the next  bar at this stage.
    secondNote.m_start = barLineTicks;
    secondNote.m_duration = secondNote.m_end - secondNote.m_start;
    secondNote.SetTimeVal(tpq);

    // Insert bar line, tie and second note
    ++it;
    it = events.insert(it, MakeBarLine(barLineTicks));
    ++it;
    it = events.insert(it, MakeTie(barLineTicks));
    ++it;
    it = events.insert(it, secondNote);

    bar++;
    barLineTicks += ticksForOneBar; 
    // loop and chop second note as before if required
  }
  return bar;
}

void InsertBarLines(int tpq, TimeSig ts, Events& events)
{
  // We don't know where the bar lines should fall (although after the 
  //  last note is probable, i.e. the last event is unlikely to be a 
  //  rest??) - so let the user specify an anacrusis, otherwise just
  //  start adding bar lines every time sig worth of tpq.

  int ticksForOneBar = static_cast<float>(tpq) * BeatsInBar(ts);
  int bar = 1; // don't add barline at start  
  bool chord = false; // true if we are parsing between ( ) chord markers
  for (auto it = events.begin(); it != events.end(); ++it)
  {
    if (it->IsChordStart()) chord = true;
    if (it->IsChordEnd()) chord = false;
    if (!it->IsNote()) continue;

    // Number of ticks at which we should insert bar line
    int barLineTicks = bar * ticksForOneBar;

    if (it->m_start < barLineTicks && it->m_end > barLineTicks)
    {
      // Note/chord duration crosses bar line, so split and tie it
      if (chord)
        bar += SplitChord(tpq, events, it, barLineTicks, ticksForOneBar);
      else
        bar += SplitNote(tpq, events, it, barLineTicks, ticksForOneBar);
    }
    else if (it->m_start >= barLineTicks)
    { 
      // We don't need to split this note or chord.
      if (chord)
      {
        // Put the new bar line before the chord marker
        it = events.insert(it - 1, MakeBarLine(barLineTicks));
        ++it;
      }
      else
      {
        it = events.insert(it, MakeBarLine(barLineTicks));
      }
      bar++;
    }
  }
  // Add final bar line
  events.push_back(MakeBarLine(bar * ticksForOneBar));
}

void InsertChordMarkers(Events& events)
{
  // Look for notes with the same start time; once found, add markers 
  //  around the notes.
  for (auto it = events.begin() + 1; it != events.end(); ++it)
  {
    if (!it->IsNote()) continue;
   
    int start = (it - 1)->m_start; 
    
    if (start == it->m_start)
    {
      it = events.insert(it - 1, MakeChordStart(it->m_start));

      // Skip over all events with the same start time      
      // Store start of notes in chord
      ++it;
      auto firstNote = it;
      while (it != events.end() && it->m_start == start)
      {
        ++it;
      }

      // Sort the notes in the chord, longest duration first.
      std::sort(firstNote, it,
        [](const Event& e1, const Event& e2)
        {
          return e2.m_duration < e1.m_duration; // descending order
        }
      );

      // Chord end marker has start and end times set to the end of the
      //  longest note. It doesn't affect other processing, but is for
      //  consistency.
      int chordEndTime = firstNote->m_end;
      if (it == events.end())
      {
        events.push_back(MakeChordEnd(chordEndTime));
        break; // no more events
      }
      it = events.insert(it, MakeChordEnd(chordEndTime));
      ++it;
    }
  }
}

static Event MakeTimeSet(int tpq, int time)
{
  Event e;
  e.m_type = EventType::TIME_SET;
  e.m_start = time;
  e.m_end = time;
  e.m_duration = 0;
  // For the final output, we use number of crotchets, which can be 
  //  fractional, of course. Alternatively, we could output the tpq
  //  value and the time in tpq ticks.
  e.m_timeSetVal = static_cast<float>(time) / static_cast<float>(tpq);
  return e;
}

void InsertTimeSetEvents(int tpq, Events& events)
{
  bool chord = false;
  int prevEventEnd = 0;

  for (auto it = events.begin(); it != events.end(); ++it)
  {
    if (it->IsChordStart()) chord = true;

    // Insert a time set event if the start time of the current
    //  event is not the same as the end time of the previous event.
    // Don't bother inside chord markers, as every note has the same
    //  start time, and so the usual rule doesn't apply.
    // Don't try to time set bar lines - they have fixed start times.
    if (   !chord 
        && it->m_start != prevEventEnd
        && !it->IsBarLine())
    {
std::cout << "// * Inserting a time set event! This event: \""
  << it->ToString()
  << "\" has start time: " 
  << it->m_start 
  << ", but prev end time is " 
  << prevEventEnd 
  << "..\n";
      it = events.insert(it, MakeTimeSet(tpq, it->m_start));
    }

    prevEventEnd = it->m_end;
    if (it->IsChordEnd()) chord = false;
  } 
}
}


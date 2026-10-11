#include <iostream>
#include "Ornament.h"

namespace MidiScore
{
int GetOrnamentNoteMaxTicks(int tpq, TimeVal quant)
{
  int m = CalcTpqMultipleForTimeVal(tpq, quant);
  // TODO check this.. if quant is qq, any note less than qqq 
  //  is a grace note? Maybe we want m * 2 / 3.
  return m / 2; // Half
//  return m * 2 / 3; // Two-thirds?
  return m * 3 / 4; // Three quarters?!
}

int OrnamentCounts::total() const
{
  return static_cast<int>(
    grace_notes.size() + trills.size() + mordents.size() + turns.size());
}

std::string Stringify(const Positions& p, const std::string& s)
{
  auto n = p.size();
  if (n == 0) return "";
  return std::to_string(n) + " " + s + ((n > 1) ? "s" : "") + " ";
}

std::string OrnamentCounts::ToString() const
{
  std::string res;

  res += Stringify(grace_notes, "grace note");
  res += Stringify(trills, "trill");
  res += Stringify(mordents, "mordent");
  res += Stringify(turns, "turn");

  return res;
}

std::vector<OrnamentCounts> OrnamentAnalyzer::GetOrnamentationPerBar(
    smf::MidiFile& midifile,
    int trackIndex,
    int totalBars,
    int ticksPerBar,
    // Maximum duration for a note to be considered part of an ornament,
    //  in ticks.
    int maxOrnamentNoteDurationTicks)
{
    std::vector<OrnamentCounts> barReports(totalBars);

    if (trackIndex < 0 || trackIndex >= midifile.getTrackCount()) 
    {
        std::cout << "Track index " << trackIndex << " out of range, no ornaments found.\n";
        return barReports;
    }

    smf::MidiEventList& track = midifile[trackIndex];
    std::vector<Note> notes = extractNotes(track);

    size_t i = 0;
    while (i < notes.size()) {
        //double dur = notes[i].end_time - notes[i].start_time;
        int durTicks = notes[i].end_tick - notes[i].start_tick;

        if (durTicks > maxOrnamentNoteDurationTicks) {
            //std::cout << "ORN: note " << i << " longer than orn length, skipping.\n";
            i++;
            continue;
        }

        // Group consecutive short notes
        size_t seqEnd = i;
        while (seqEnd + 1 < notes.size()) {
            //double nextDur = notes[seqEnd + 1].end_time - notes[seqEnd + 1].start_time;
            int nextDurTicks = notes[seqEnd + 1].end_tick - notes[seqEnd + 1].start_tick;
            // Extended max duration: after the first short note, 
            //  subsequent notes in the ornament can be longer. 
            int extMaxDuration = maxOrnamentNoteDurationTicks * 3 / 2;
            // TODO Gap is still in absolute seconds... 
            double gap = notes[seqEnd + 1].start_time - notes[seqEnd].end_time;

            if (nextDurTicks < extMaxDuration && gap < 0.05) {
                seqEnd++;
            } else {
                break;
            }
        }

        OrnamentType type = classifySequence(notes, i, seqEnd, maxOrnamentNoteDurationTicks);

        if (type != OrnamentType::None) {
            int barIndex = notes[i].start_tick / ticksPerBar;
            if (barIndex >= 0 && barIndex < totalBars) 
            {
                int start = notes[i].start_tick;
                int end = notes[i].end_tick;
                recordOrnament(barReports[barIndex], type, start, end);
            }
            i = seqEnd + 1; // Skip past identified ornament
        } else {
            i++;
        }
    }

    return barReports;
}

OrnamentType OrnamentAnalyzer::classifySequence(
    const std::vector<Note>& notes,
    size_t start,
    size_t end,
    int maxGraceDurationTicks)
{
    size_t count = end - start + 1;

    // 1. Single rapid note before a main note -> Grace Note
    if (count == 1) {
        if (start + 1 < notes.size()) {
            int pitchDiff = std::abs(notes[start].pitch - notes[start + 1].pitch);
            //double timeToNext = notes[start + 1].start_time - notes[start].start_time;
            int timeToNextTicks = notes[start + 1].start_tick - notes[start].start_tick;
            if (pitchDiff <= 2 && timeToNextTicks <= maxGraceDurationTicks) {
                return OrnamentType::GraceNote;
            }
        }
        return OrnamentType::None;
    }

    // Calculate consecutive interval steps between adjacent notes
    std::vector<int> steps;
    for (size_t k = start; k < end; ++k) {
        steps.push_back(notes[k + 1].pitch - notes[k].pitch);
    }

    auto validStep = [](int s) { return std::abs(s) >= 1 && std::abs(s) <= 2; };

    // 2. Three notes (2 steps) -> Mordent [+step, -step] or [-step, +step]
    if (count == 3) {
        if (steps[0] == -steps[1] && validStep(steps[0])) {
            return OrnamentType::Mordent;
        }
    }
    // 3. Four notes (3 steps) -> Turn (Standard or Inverted)
    if (count == 4 && validStep(steps[0]) && validStep(steps[1]) && validStep(steps[2])) {
        bool standardTurn = (steps[0] < 0 && steps[1] < 0 && steps[2] > 0);
        bool invertedTurn = (steps[0] > 0 && steps[1] > 0 && steps[2] < 0);

        if (standardTurn || invertedTurn) {
            return OrnamentType::Turn;
        }
    }

    // 4. Five notes (4 steps) -> 5-Note Turn starting on principal note
    if (count == 5 && validStep(steps[0]) && validStep(steps[1]) && validStep(steps[2]) && validStep(steps[3])) {
        bool fiveNoteTurn = (steps[0] > 0 && steps[1] < 0 && steps[2] < 0 && steps[3] > 0);
        if (fiveNoteTurn) {
            return OrnamentType::Turn;
        }
    }

    // 5. Four or more alternating notes -> Trill
    if (count >= 4) {
        bool isAlternating = true;
        for (size_t k = 0; k < steps.size(); ++k) {
            if (!validStep(steps[k])) {
                isAlternating = false;
                break;
            }
            if (k > 0 && ((steps[k] > 0) == (steps[k - 1] > 0))) {
                isAlternating = false; // Same direction twice = not alternating
                break;
            }
        }
        if (isAlternating) {
            return OrnamentType::Trill;
        }
    }

    return OrnamentType::None;
}
}


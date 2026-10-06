#include <iostream>
#include "Ornament.h"

namespace MidiScore
{
std::string Stringify(int n, const std::string& s)
{
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
        double maxOrnamentNoteDuration)
    {
        std::vector<OrnamentCounts> barReports(totalBars);

        if (trackIndex < 0 || trackIndex >= midifile.getTrackCount()) {
            return barReports;
        }

        smf::MidiEventList& track = midifile[trackIndex];
        std::vector<Note> notes = extractNotes(track);

        size_t i = 0;
        while (i < notes.size()) {
            double dur = notes[i].end_time - notes[i].start_time;

            if (dur >= maxOrnamentNoteDuration) {
                //std::cout << "ORN: note " << i << " longer than orn length, skipping.\n";
                i++;
                continue;
            }

            // Group consecutive short notes
            size_t seqEnd = i;
            while (seqEnd + 1 < notes.size()) {
                double nextDur = notes[seqEnd + 1].end_time - notes[seqEnd + 1].start_time;
                double gap = notes[seqEnd + 1].start_time - notes[seqEnd].end_time;

                if (nextDur < maxOrnamentNoteDuration * 1.5 && gap < 0.05) {
                    seqEnd++;
                } else {
                    break;
                }
            }

            OrnamentType type = classifySequence(notes, i, seqEnd, maxOrnamentNoteDuration);

            if (type != OrnamentType::None) {
                int barIndex = notes[i].start_tick / ticksPerBar;
                if (barIndex >= 0 && barIndex < totalBars) {
                    recordOrnament(barReports[barIndex], type);
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
        double maxGraceDuration)
    {
        size_t count = end - start + 1;

        // 1. Single rapid note before a main note -> Grace Note
        if (count == 1) {
            if (start + 1 < notes.size()) {
                int pitchDiff = std::abs(notes[start].pitch - notes[start + 1].pitch);
                double timeToNext = notes[start + 1].start_time - notes[start].start_time;
                if (pitchDiff <= 2 && timeToNext <= maxGraceDuration) {
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


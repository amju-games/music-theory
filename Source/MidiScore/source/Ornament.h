#pragma once

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>
#include "MidiFile.h"
#include "TimeSig.h"
#include "TimeVal.h"

namespace MidiScore
{
// Start and end time (ticks) of one ornament
using StartEnd = std::pair<int, int>;
// Positions of multiple ornaments
using Positions = std::vector<StartEnd>;

struct OrnamentCounts 
{
    std::string ToString() const;

    Positions grace_notes;
    Positions trills;
    Positions mordents;
    Positions turns;

    // Get total number of ornaments
    int total() const;
};

enum class OrnamentType {
    None,
    GraceNote,
    Mordent,
    Turn,
    Trill
};

// Helper function: get the max number of ticks for a note to be
//  considered part of an ornament.
int GetOrnamentNoteMaxTicks(int tpq, TimeVal quant);

class OrnamentAnalyzer 
{
public:
    // Find ornaments in each bar for the given track.
    //
    // maxOrnNoteDurTicks: 
    // Maximum duration for a note to be considered part of an ornament,
    //  in ticks.
    // Outputs a report of the positions of each ornament found.
    static std::vector<OrnamentCounts> GetOrnamentationPerBar(
        smf::MidiFile& midifile,
        int trackIndex,
        int totalBars,
        int ticksPerBar,
        int maxOrnNoteDurTicks);

private:
    struct Note {
        int pitch;
        int velocity;
        int channel;
        int start_tick;
        int end_tick;
        double start_time;
        double end_time;
    };

    static OrnamentType classifySequence(
        const std::vector<Note>& notes,
        size_t start,
        size_t end,
        int maxGraceDurationTicks);

    static void recordOrnament(
        OrnamentCounts& counts, 
        OrnamentType type, int start, int end) {
        switch (type) {
            case OrnamentType::GraceNote: 
              counts.grace_notes.emplace_back(StartEnd{start, end}); 
              break;

            case OrnamentType::Trill:     
              counts.trills.emplace_back(StartEnd{start, end});
              break;

            case OrnamentType::Mordent:   
              counts.mordents.emplace_back(StartEnd{start, end});
              break;

            case OrnamentType::Turn:      
              counts.turns.emplace_back(StartEnd{start, end});
              break;

            default: break;
        }
    }

    static std::vector<Note> extractNotes(smf::MidiEventList& track) {
        std::vector<Note> notes;
        track.linkNotePairs();

        for (int i = 0; i < track.size(); ++i) {
            smf::MidiEvent& ev = track[i];
            if (ev.isNoteOn() && ev.isLinked()) {
                smf::MidiEvent* offEv = ev.getLinkedEvent();
                notes.push_back({
                    ev.getKeyNumber(),
                    ev.getVelocity(),
                    ev.getChannel(),
                    ev.tick,
                    offEv->tick,
                    ev.seconds,
                    offEv->seconds
                });
            }
        }
        return notes;
    }
};
}


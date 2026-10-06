#pragma once

#include "MidiFile.h"
#include <vector>
#include <cmath>
#include <algorithm>

namespace MidiScore
{
struct OrnamentCounts 
{
    std::string ToString() const;

    int grace_notes{0};
    int trills{0};
    int mordents{0};
    int turns{0};

    int total() const {
        return grace_notes + trills + mordents + turns;
    }
};

enum class OrnamentType {
    None,
    GraceNote,
    Mordent,
    Turn,
    Trill
};

class OrnamentAnalyzer {
public:
    struct Note {
        int pitch;
        int velocity;
        int channel;
        int start_tick;
        int end_tick;
        double start_time;
        double end_time;
    };

    static std::vector<OrnamentCounts> GetOrnamentationPerBar(
        smf::MidiFile& midifile,
        int trackIndex,
        int totalBars,
        int ticksPerBar,
        double maxOrnamentNoteDuration = 0.08);

private:
    static OrnamentType classifySequence(
        const std::vector<Note>& notes,
        size_t start,
        size_t end,
        double maxGraceDuration);

    static void recordOrnament(OrnamentCounts& counts, OrnamentType type) {
        switch (type) {
            case OrnamentType::GraceNote: counts.grace_notes++; break;
            case OrnamentType::Trill:     counts.trills++; break;
            case OrnamentType::Mordent:   counts.mordents++; break;
            case OrnamentType::Turn:      counts.turns++; break;
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


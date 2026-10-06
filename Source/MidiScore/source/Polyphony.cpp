#include <vector>
#include <algorithm>
#include <cmath>
#include <MidiFile.h>
#include "NumBars.h"
#include "TimeSig.h"

namespace MidiScore
{
std::vector<int> GetPolyphonyLevelPerBar(
    smf::MidiFile& midifile,
    int track,
    TimeSig ts)
{
    midifile.linkNotePairs();

    if (track < 0 || track >= midifile.getTrackCount()) {
        return {};
    }

    smf::MidiEventList& eventList = midifile[track];
    if (eventList.size() == 0) {
        return {};
    }

    // 1. Calculate length of one bar in MIDI ticks
    int tpq = midifile.getTPQ(); // Ticks Per Quarter note
    int ticksPerBar = static_cast<int>(
        tpq * Numerator(ts) * (4.0 / Denominator(ts)));
    if (ticksPerBar <= 0) return {};

    // 2. Find total duration to determine the number of bars
    int maxTick = 0;
    for (int i = 0; i < eventList.size(); ++i) {
        if (eventList[i].isNoteOn() || eventList[i].isNoteOff()) {
            maxTick = std::max(maxTick, eventList[i].tick);
        }
    }

    if (maxTick == 0) return {};

    int totalBars = (maxTick + ticksPerBar - 1) / ticksPerBar;
    std::vector<int> polyphonyPerBar(totalBars, 0);

    // 3. Build sweep-line events per bar
    struct SweepEvent {
        int tick;
        int delta; // +1 for note onset, -1 for note release
    };

    std::vector<std::vector<SweepEvent>> barEvents(totalBars);

    for (int i = 0; i < eventList.size(); ++i) {
        smf::MidiEvent& ev = eventList[i];
        if (ev.isNoteOn() && ev.isLinked()) {
            int startTick = ev.tick;
            int endTick = ev.getLinkedEvent()->tick;

            if (startTick >= endTick) continue;

            // Determine which bars this note overlaps
            int startBar = startTick / ticksPerBar;
            int endBar = (endTick - 1) / ticksPerBar;

            startBar = std::max(0, std::min(startBar, totalBars - 1));
            endBar = std::max(0, std::min(endBar, totalBars - 1));

            // Clip note intervals to each overlapping bar boundary
            for (int b = startBar; b <= endBar; ++b) {
                int barStart = b * ticksPerBar;
                int barEnd = (b + 1) * ticksPerBar;

                int clippedStart = std::max(startTick, barStart);
                int clippedEnd = std::min(endTick, barEnd);

                if (clippedStart < clippedEnd) {
                    barEvents[b].push_back({clippedStart, +1});
                    barEvents[b].push_back({clippedEnd, -1});
                }
            }
        }
    }

    // 4. Sweep each bar to find peak active polyphony
    for (int b = 0; b < totalBars; ++b) {
        auto& events = barEvents[b];

        // Sort events chronologically.
        // Tie-breaker: -1 comes before +1 so sequential notes on the same tick 
        // don't momentarily register as polyphonic.
        std::sort(events.begin(), events.end(), [](const SweepEvent& a, const SweepEvent& bEv) {
            if (a.tick != bEv.tick) return a.tick < bEv.tick;
            return a.delta < bEv.delta; 
        });

        int currentPolyphony = 0;
        int maxPolyphony = 0;

        for (const auto& ev : events) {
            currentPolyphony += ev.delta;
            maxPolyphony = std::max(maxPolyphony, currentPolyphony);
        }

        polyphonyPerBar[b] = maxPolyphony;
    }

    return polyphonyPerBar;
}

namespace
{
    struct Note {
        int pitch;
        int velocity;
        int channel;
        int start_tick;
        int end_tick;
    };
}

    // Splits trackIndex into multiple monophonic tracks. Returns new track indices.
    std::vector<int> SplitPolyTrack(
        smf::MidiFile& midifile,
        int trackIndex,
        TimeSig ts)
    {
        // 1. Determine maximum required voices
        std::vector<int> polyphonyPerBar = GetPolyphonyLevelPerBar(midifile, trackIndex, ts);
        int maxVoices = 0;
        for (int p : polyphonyPerBar) {
            maxVoices = std::max(maxVoices, p);
        }

        // If track is already monophonic or empty, no splitting needed
        if (maxVoices <= 1) {
            return {trackIndex};
        }

        // 2. Extract active notes from track
        smf::MidiEventList& track = midifile[trackIndex];
        track.linkNotePairs();

        std::vector<Note> notes;
        for (int i = 0; i < track.size(); ++i) {
            if (track[i].isNoteOn() && track[i].isLinked()) {
                notes.push_back({
                    track[i].getKeyNumber(),
                    track[i].getVelocity(),
                    track[i].getChannel(),
                    track[i].tick,
                    track[i].getLinkedEvent()->tick
                });
            }
        }

        // Sort notes: chronologically first, then highest pitch first for simultaneous notes
        std::sort(notes.begin(), notes.end(), [](const Note& a, const Note& b) {
            if (a.start_tick != b.start_tick) return a.start_tick < b.start_tick;
            return a.pitch > b.pitch; // Top pitch priority (Soprano -> Bass)
        });

        // 3. Allocate notes into monophonic voice streams
        std::vector<std::vector<Note>> voiceStreams(maxVoices);
        std::vector<int> voiceLastEndTick(maxVoices, -1);

        for (const auto& note : notes) {
            int assignedVoice = -1;

            // Find the highest-priority voice stream that is free
            for (int v = 0; v < maxVoices; ++v) {
                if (voiceLastEndTick[v] <= note.start_tick) {
                    assignedVoice = v;
                    break;
                }
            }

            // Fallback to last voice if all streams are occupied
            if (assignedVoice == -1) {
                assignedVoice = maxVoices - 1;
            }

            voiceStreams[assignedVoice].push_back(note);
            voiceLastEndTick[assignedVoice] = std::max(voiceLastEndTick[assignedVoice], note.end_tick);
        }

        // 4. Create new monophonic tracks in midifile
        std::vector<int> newTrackIndices;

        for (int v = 0; v < maxVoices; ++v) {
            if (voiceStreams[v].empty()) continue;

            int newTrackIdx = midifile.addTrack();
            newTrackIndices.push_back(newTrackIdx);
            smf::MidiEventList& newTrack = midifile[newTrackIdx];

            for (const auto& n : voiceStreams[v]) {
                smf::MidiEvent onEvent, offEvent;

                onEvent.makeNoteOn(n.channel, n.pitch, n.velocity);
                onEvent.tick = n.start_tick;

                offEvent.makeNoteOff(n.channel, n.pitch);
                offEvent.tick = n.end_tick;

                newTrack.append(onEvent);
                newTrack.append(offEvent);
            }

            midifile.sortTrack(newTrackIdx);
            midifile[newTrackIdx].linkNotePairs();
        }

        return newTrackIndices;
    }
}


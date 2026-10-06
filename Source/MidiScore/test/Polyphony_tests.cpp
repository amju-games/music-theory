#include "catch.hpp"
#include "MidiFile.h"
#include "Polyphony.h" 

using namespace MidiScore;

// Helper to construct a MidiFile with specific note ticks (using default 480 TPQ)
smf::MidiFile createMockMidiFileWithTicks(
    const std::vector<std::tuple<int, int, int>>& notes, // pitch, startTick, endTick
    int tpq = 480) 
{
    smf::MidiFile midi;
    midi.setTicksPerQuarterNote(tpq);
    midi.addTrack(1); // Track 0

    for (const auto& [pitch, startTick, endTick] : notes) {
        smf::MidiEvent onEvent, offEvent;

        onEvent.makeNoteOn(0, pitch, 100);
        onEvent.tick = startTick;

        offEvent.makeNoteOff(0, pitch);
        offEvent.tick = endTick;

        midi[0].append(onEvent);
        midi[0].append(offEvent);
    }

    midi.sortTrack(0);
    midi.linkNotePairs();
    return midi;
}

TEST_CASE("GetPolyphonyLevelPerBar: Empty or invalid track returns empty vector", "[Polyphony]") {
    smf::MidiFile midi;
    midi.addTrack(1);
    TimeSig ts = TimeSig::TS_4_4;

    SECTION("Empty track") {
        auto polyphony = GetPolyphonyLevelPerBar(midi, 0, ts);
        CHECK(polyphony.empty());
    }

    SECTION("Out of bounds track index") {
        auto polyphony = GetPolyphonyLevelPerBar(midi, 99, ts);
        CHECK(polyphony.empty());
    }
}

TEST_CASE("GetPolyphonyLevelPerBar: Strictly monophonic track", "[Polyphony]") {
    //TimeSig ts{4, 4}; // 480 TPQ * 4 = 1920 ticks per bar
    TimeSig ts = TimeSig::TS_4_4;
    
    // Notes sequential without overlap in Bar 0 (ticks 0 to 1920)
    std::vector<std::tuple<int, int, int>> notes = {
        {60, 0, 480},
        {62, 480, 960},
        {64, 960, 1440}
    };

    smf::MidiFile midi = createMockMidiFileWithTicks(notes);
    auto polyphony = GetPolyphonyLevelPerBar(midi, 0, ts);

    REQUIRE(polyphony.size() == 1);
    CHECK(polyphony[0] == 1);
}

TEST_CASE("GetPolyphonyLevelPerBar: Back-to-back notes on exact tick boundaries", "[Polyphony]") {
    //TimeSig ts{4, 4};
    TimeSig ts = TimeSig::TS_4_4;

    // Note 1 ends at tick 480, Note 2 begins at tick 480.
    // Tie-breaker must ensure this is counted as polyphony 1, not 2.
    std::vector<std::tuple<int, int, int>> notes = {
        {60, 0, 480},
        {62, 480, 960}
    };

    smf::MidiFile midi = createMockMidiFileWithTicks(notes);
    auto polyphony = GetPolyphonyLevelPerBar(midi, 0, ts);

    REQUIRE(polyphony.size() == 1);
    CHECK(polyphony[0] == 1);
}

TEST_CASE("GetPolyphonyLevelPerBar: Chords and staggered overlap within one bar", "[Polyphony]") {
    //TimeSig ts{4, 4};
    TimeSig ts = TimeSig::TS_4_4;

    // 3-note triad played simultaneously + a staggered 4th note
    std::vector<std::tuple<int, int, int>> notes = {
        {60, 0, 960},   // C4
        {64, 0, 960},   // E4
        {67, 0, 960},   // G4
        {72, 480, 1200} // C5 overlaps from tick 480 to 960 with the triad -> peak 4
    };

    smf::MidiFile midi = createMockMidiFileWithTicks(notes);
    auto polyphony = GetPolyphonyLevelPerBar(midi, 0, ts);

    REQUIRE(polyphony.size() == 1);
    CHECK(polyphony[0] == 4);
}

TEST_CASE("GetPolyphonyLevelPerBar: Notes spanning across bar boundaries", "[Polyphony]") {
    //TimeSig ts{4, 4}; // 1920 ticks per bar
    TimeSig ts = TimeSig::TS_4_4;

    // Note starts in Bar 0 (tick 1000) and extends into Bar 1 (tick 2500)
    std::vector<std::tuple<int, int, int>> notes = {
        {60, 0, 500},       // Bar 0 note
        {62, 1000, 2500},   // Spans Bar 0 (1000..1920) and Bar 1 (1920..2500)
        {64, 2000, 2400}    // Bar 1 note overlapping the sustained note -> peak 2 in Bar 1
    };

    smf::MidiFile midi = createMockMidiFileWithTicks(notes);
    auto polyphony = GetPolyphonyLevelPerBar(midi, 0, ts);

    REQUIRE(polyphony.size() == 2);
    CHECK(polyphony[0] == 1); // Bar 0 peak polyphony = 1
    CHECK(polyphony[1] == 2); // Bar 1 peak polyphony = 2 (sustained note + second note)
}

TEST_CASE("GetPolyphonyLevelPerBar: Different time signatures (3/4 time)", "[Polyphony]") {
//    TimeSig ts{3, 4}; // 480 TPQ * 3 = 1440 ticks per bar
    TimeSig ts = TimeSig::TS_3_4;

    std::vector<std::tuple<int, int, int>> notes = {
        {60, 0, 1200},   // Bar 0
        {62, 1440, 2000} // Bar 1 onset exactly at start of Bar 1 (tick 1440)
    };

    smf::MidiFile midi = createMockMidiFileWithTicks(notes);
    auto polyphony = GetPolyphonyLevelPerBar(midi, 0, ts);

    REQUIRE(polyphony.size() == 2);
    CHECK(polyphony[0] == 1);
    CHECK(polyphony[1] == 1);
}

TEST_CASE("Splits a 3-note chord into 3 monophonic tracks", "[SplitPolyTrack]") {
    smf::MidiFile midi;
    midi.setTicksPerQuarterNote(480);
    midi.addTrack(1);

    // C Major Triad played simultaneously at tick 0..480
    // C4 (60), E4 (64), G4 (67)
    std::vector<int> pitches = {60, 64, 67};
    for (int p : pitches) {
        smf::MidiEvent on, off;
        on.makeNoteOn(0, p, 100);  on.tick = 0;
        off.makeNoteOff(0, p);     off.tick = 480;
        midi[0].append(on);
        midi[0].append(off);
    }
    midi.sortTrack(0);

    TimeSig ts = TimeSig::TS_4_4;
    std::vector<int> newTracks = SplitPolyTrack(midi, 0, ts);

    REQUIRE(newTracks.size() == 3);

    // Check that Voice 0 got the highest pitch (G4 = 67)
    midi[newTracks[0]].linkNotePairs();
    CHECK(midi[newTracks[0]][0].getKeyNumber() == 67);

    // Check that Voice 1 got middle pitch (E4 = 64)
    midi[newTracks[1]].linkNotePairs();
    CHECK(midi[newTracks[1]][0].getKeyNumber() == 64);

    // Check that Voice 2 got lowest pitch (C4 = 60)
    midi[newTracks[2]].linkNotePairs();
    CHECK(midi[newTracks[2]][0].getKeyNumber() == 60);
}

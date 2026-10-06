#include "catch.hpp"
#include "Ornament.h"

using namespace MidiScore;

// Helper function to append a note to a MIDI track
void appendTestNote(smf::MidiFile& midi, int track, int pitch, double startTimeSec, double durationSec, int ticksPerQuarter = 480) {
    smf::MidiEvent on, off;
    on.makeNoteOn(0, pitch, 100);
    on.seconds = startTimeSec;
    on.tick = static_cast<int>(startTimeSec * 2.0 * ticksPerQuarter); // assumes 120 BPM (2 ticks/ms @ 480 TPQ)

    off.makeNoteOff(0, pitch);
    off.seconds = startTimeSec + durationSec;
    off.tick = static_cast<int>((startTimeSec + durationSec) * 2.0 * ticksPerQuarter);

    midi[track].append(on);
    midi[track].append(off);
}

TEST_CASE("OrnamentAnalyzer: Comprehensive multi-bar ornament detection", "[OrnamentAnalyzer]") {
    smf::MidiFile midi;
    midi.setTicksPerQuarterNote(480);
    midi.addTrack(1);

    int ticksPerBar = 1920; // 4/4 time at 480 TPQ

    // ------------------------------------------------------------------
    // BAR 0: No Ornaments (Clean quarter notes)
    // ------------------------------------------------------------------
    appendTestNote(midi, 0, 60, 0.00, 0.45);
    appendTestNote(midi, 0, 62, 0.50, 0.45);
    appendTestNote(midi, 0, 64, 1.00, 0.45);
    appendTestNote(midi, 0, 65, 1.50, 0.45);

    // ------------------------------------------------------------------
    // BAR 1: Multiple Ornaments (Grace Note + Upper Mordent)
    // ------------------------------------------------------------------
    // Grace note (61 -> 60)
    appendTestNote(midi, 0, 61, 2.00, 0.04);
    appendTestNote(midi, 0, 60, 2.05, 0.40);

    // Upper Mordent (64 -> 66 -> 64)
    appendTestNote(midi, 0, 64, 2.60, 0.04);
    appendTestNote(midi, 0, 66, 2.65, 0.04);
    appendTestNote(midi, 0, 64, 2.70, 0.04);

    // ------------------------------------------------------------------
    // BAR 2: Turn (D4 -> C4 -> B3 -> C4)
    // ------------------------------------------------------------------
    appendTestNote(midi, 0, 62, 4.00, 0.04);
    appendTestNote(midi, 0, 60, 4.05, 0.04);
    appendTestNote(midi, 0, 59, 4.10, 0.04);
    appendTestNote(midi, 0, 60, 4.15, 0.04);

    // ------------------------------------------------------------------
    // BAR 3: Trill (C4 -> D4 -> C4 -> D4 -> C4)
    // ------------------------------------------------------------------
    appendTestNote(midi, 0, 60, 6.00, 0.04);
    appendTestNote(midi, 0, 62, 6.05, 0.04);
    appendTestNote(midi, 0, 60, 6.10, 0.04);
    appendTestNote(midi, 0, 62, 6.15, 0.04);
    appendTestNote(midi, 0, 60, 6.20, 0.04);

    midi.sortTrack(0);

    auto reports = OrnamentAnalyzer::GetOrnamentationPerBar(midi, 0, 4, ticksPerBar);

    REQUIRE(reports.size() == 4);

    // Bar 0: Clean bar
    SECTION("Bar 0 contains zero ornaments") {
        CHECK(reports[0].total() == 0);
    }

    // Bar 1: Multiple ornaments
    SECTION("Bar 1 contains 1 Grace Note and 1 Mordent") {
        CHECK(reports[1].grace_notes == 1);
        CHECK(reports[1].mordents == 1);
        CHECK(reports[1].total() == 2);
    }

    // Bar 2: Turn
    SECTION("Bar 2 contains 1 Turn") {
        CHECK(reports[2].turns == 1);
        CHECK(reports[2].total() == 1);
    }

    // Bar 3: Trill
    SECTION("Bar 3 contains 1 Trill") {
        CHECK(reports[3].trills == 1);
        CHECK(reports[3].total() == 1);
    }
}


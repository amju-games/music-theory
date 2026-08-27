#include "catch.hpp"
#include "Pitch.h"

using namespace MidiScore;

TEST_CASE("PitchToString handles out-of-bounds MIDI pitches", "[PitchToString]") {
    REQUIRE(PitchToString(-1, KeySig::KS_SHARP_0) == "Invalid");
    REQUIRE(PitchToString(128, KeySig::KS_FLAT_1) == "Invalid");
}

TEST_CASE("PitchToString handles extreme MIDI boundaries", "[PitchToString]") {
    // Pitch 0 = C-1
    REQUIRE(PitchToString(0, KeySig::KS_SHARP_0) == "c-1");
    // Pitch 127 = G9
    REQUIRE(PitchToString(127, KeySig::KS_FLAT_0) == "g9");
}

TEST_CASE("PitchToString formats natural notes identically across key signatures", "[PitchToString]") {
    SECTION("Middle C (Pitch 60)") {
        REQUIRE(PitchToString(60, KeySig::KS_SHARP_3) == "c4");
        REQUIRE(PitchToString(60, KeySig::KS_FLAT_4) == "c4");
    }
    
    SECTION("A440 (Pitch 69)") {
        REQUIRE(PitchToString(69, KeySig::KS_SHARP_0) == "a4");
        REQUIRE(PitchToString(69, KeySig::KS_FLAT_7) == "a4");
    }
}

TEST_CASE("PitchToString formats accidentals based on key signature", "[PitchToString]") {
    SECTION("Sharp key signature (e.g., D Major)") {
        REQUIRE(PitchToString(61, KeySig::KS_SHARP_2) == "c4+");
        REQUIRE(PitchToString(63, KeySig::KS_SHARP_2) == "d4+");
        REQUIRE(PitchToString(66, KeySig::KS_SHARP_2) == "f4+");
        REQUIRE(PitchToString(68, KeySig::KS_SHARP_2) == "g4+");
        REQUIRE(PitchToString(70, KeySig::KS_SHARP_2) == "a4+");
    }

    SECTION("Flat key signature (e.g., Eb Major)") {
        REQUIRE(PitchToString(61, KeySig::KS_FLAT_3) == "d4-");
        REQUIRE(PitchToString(63, KeySig::KS_FLAT_3) == "e4-");
        REQUIRE(PitchToString(66, KeySig::KS_FLAT_3) == "g4-");
        REQUIRE(PitchToString(68, KeySig::KS_FLAT_3) == "a4-");
        REQUIRE(PitchToString(70, KeySig::KS_FLAT_3) == "b4-");
    }
}

TEST_CASE("PitchToString treats FLAT_0 key sig as flat-defaulting", "[PitchToString]") {
    REQUIRE(PitchToString(61, KeySig::KS_SHARP_0) == "c4+");
    REQUIRE(PitchToString(61, KeySig::KS_FLAT_0) == "d4-");
}


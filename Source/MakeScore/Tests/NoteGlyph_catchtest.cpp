#include <iostream>
#include "catch.hpp"
#include "NoteGlyph.h"

TEST_CASE("Calc stave line for midi pitch", "[NoteGlyph]")
{
  // Middle c, treb clef, 0 sharps or flats
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_TREBLE, Pitch(60)) == -2);

  // Middle c, bass clef, 0 sharps or flats
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_BASS, Pitch(60)) == 10);

  // f5, treb clef, 0 sharps or flats
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_TREBLE, Pitch(77)) == 8);

  // g2, bass clef, 0 sharps or flats
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_BASS, Pitch(43)) == 0);
}

TEST_CASE("Calc stave line - midi pitches - enharmonic notes", "[NoteGlyph]")
{
  // Midi pitches are placed according to key sig: this is done automatically to hopefully
  //  be helpful and reduce the number of input tokens required. To override this behaviour,
  //  pitches can be specified as step/octave/alter.

  // Pitch 66 treated as f4#, treb clef, sharps key
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_5_SHARP, Clef::CLEF_TREBLE, Pitch(66)) == 1);

  // Pitch 66 treated as gb4, treb clef, flats key
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_5_FLAT, Clef::CLEF_TREBLE, Pitch(66)) == 2);

  // Pitch 58 treated as a#3, bass clef, sh key
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_1_SHARP, Clef::CLEF_BASS, Pitch(58)) == 8);

  // Pitch 58 treated as bb3, bass clef, flat key
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_1_FLAT, Clef::CLEF_BASS, Pitch(58)) == 9);
}

TEST_CASE("Calc stave line - step/oct/alter pitches", "[NoteGlyph]")
{
  // Middle c, treb clef, 0 sharps or flats
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_TREBLE, Pitch("c", 4, 0)) == -2);
  // Check alter value doesn't change the stave pos
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_TREBLE, Pitch("c", 4, 2)) == -2);
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_TREBLE, Pitch("c", 4, -1)) == -2);

  // f5, treb clef
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_TREBLE, Pitch("f", 5, 0)) == 8);

  // middle c, bass clef
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_BASS, Pitch("c", 4, 0)) == 10);

  // g2, bass clef
  REQUIRE(NoteGlyph::CalcStaveLine(KeySig::KEYSIG_0_SHARP, Clef::CLEF_BASS, Pitch("g", 2, 0)) == 0);
}


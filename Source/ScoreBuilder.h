// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

namespace Amju
{
class GuiMusicScore;

// * Note *
// Pitch and duration of a note.
struct Note
{
  static const int BREVE = 256;
  static const int SEMIBREVE = 128;
  static const int MINIM = 64;
  static const int CROTCHET = 32;
  static const int QUAVER = 16;
  static const int SEMI_QUAVER = 8;

  static const int DOTTED_MINIM = MINIM + CROTCHET;
  static const int DOTTED_CROTCHET = CROTCHET + QUAVER;
  static const int DOTTED_QUAVER = QUAVER + SEMI_QUAVER;

  //static const int DOTTED_SEMI_QUAVER = ...
  // etc

  Note(int pitch, int stavePos, int duration) : m_pitch(pitch), m_stavePos(stavePos), m_duration(duration) {}

  int m_pitch; // In semitones, 0 is middle C, 1 is C#, etc.
  int m_stavePos; // 0 is middle line of stave
  int m_duration; 
};

// * ScoreBuilder *
// Layer above GuiMusicScore, which is just a bunch of glyphs.
// In ScoreBuilder we represent the score as a sequence of notes and other
//  symbols. 
class ScoreBuilder
{
public:
  // Write the score elements to the given GuiMusicScore object.
  void Write(GuiMusicScore&);

  void Add(int stave, int time, const Note& note);

private:
  // Very simple for now
  std::vector<Note> m_notes;
};
}

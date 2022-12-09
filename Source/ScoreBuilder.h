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

  Note(int pitch, int stavePosX, int stavePosY, int duration, int bar) : 
    m_pitch(pitch), 
    m_stavePosX(stavePosX), 
    m_stavePosY(stavePosY), 
    m_duration(duration),
    m_bar(bar) {}

  int m_pitch = 0; // In semitones, 0 is middle C, 1 is C#, etc.
  // Position of note
  int m_stavePosX = 128; // Horiz position in bar, 0 (left).. 255 (right)
  int m_stavePosY = 0; // 0 is middle line of stave
  int m_duration = 0;  // e.g. CROTCHET etc
  int m_bar = 0; // 1-based bar in which note lives
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

  // Create notes etc from given string
  void SetFromString(const std::string& s);

  void Add(const Note& note, int stave = 0);

  // At start up, set the number of bars we expect to create.
  // Do we need this?
  void SetNumBars(int numBars);

  // Do we need something like this?
  void SetKeySig();

private:
  // Very simple for now
  using NoteVec = std::vector<Note>;

  // Notes in one bar
//  NoteVec m_notes;

  using Bar = NoteVec;
  using BarVec = std::vector<Bar>;
  BarVec m_bars;

  // TODO multiple staves; how to organise bars vs staves?
  
private:
  void AddBar(int barNum1Based);
  void WriteBar(const Bar& b, GuiMusicScore& gui);
};
}

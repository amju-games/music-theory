// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017
//
// * MakeScore *
//
// Sub project to convert easily-authorable music content
//  into list of glyphs.
//
// To build:
//    (Mac) clang++ *.cpp -std=c++11
//    (Win) cl *.cpp
//
// E.g. use: 
//    (Mac) echo '4/4 c c mr' | ./a.out
// Same for Win, although we need double quotes and internally we need to strip off the quotes:
//    (Win) echo "4/4 c c mr" | MakeScore.exe


// Music score coords:
//  +--------------------------+   0.4
//  +--------------------------+
//  +--------------------------+
//  +--------------------------+
//  +--------------------------+   0.0
// 0.0                     PAGE_WIDTH

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "MakeScore.h"

float Interp(float f0, float f1, float t)
{
  return f0 + (f1 - f0) * t;
}

void Trim(std::string& s)
{
  while (!s.empty() && (s[0] == ' ' || s[0] == '\r' || s[0] == '\n'))
  {
    s = s.substr(1);
  }
  while (!s.empty() && (s[s.size() - 1] == ' ' || s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n'))
  {
    s = s.substr(0, s.size() - 1);
  }
}

bool Contains(const std::string& s, char c)
{
  return std::find(s.begin(), s.end(), c) != s.end();
}

std::string Remove(std::string& s, char c)
{
  s.erase(std::remove(s.begin(), s.end(), c), s.end()); 
  return s;
}

static const std::map<std::string, float> TIME_VAL_STRS =
{
  { "sb", TIMEVAL_SEMIBREVE },
  { "m",  TIMEVAL_MINIM },
  { "c",  TIMEVAL_CROTCHET },
  { "q",  TIMEVAL_QUAVER },
  { "qq", TIMEVAL_SEMIQUAVER },
};

float GetTimeVal(std::string s)
{
  Remove(s, 'r'); // rests and notes are treated the same
  Remove(s, '*'); // in case glyph is hidden

  float dot = 1.f;
  if (Contains(s, '.'))
  {
    dot = 1.5f;
    Remove(s, '.');
  }

  auto it = TIME_VAL_STRS.find(s);
  if (it == TIME_VAL_STRS.end())
  {
    return -1;
  }

  return dot * it->second;
}

// Convenience functions

// Str - convert given argument to a string
// For floats, this results in no trailing zeros, nice
template <typename T>
std::string Str(T f)
{
  std::stringstream ss;
  ss << f;
  return ss.str();
}

bool IsBeam(const std::string& s)
{
  return Contains(s, '-') || Contains(s, '=');
}

bool IsRest(const std::string& s)
{
  return Contains(s, 'r');
}

float GetHeight(BeamLevel bl)
{
  // Relies on the int values 0, 1...
  return static_cast<float>(bl);
}

static const std::map<std::string, TimeSig> TIME_SIG_STRS = 
{
  { "2/4", TimeSig::TWO_FOUR },
  { "3/4", TimeSig::THREE_FOUR },
  { "4/4", TimeSig::FOUR_FOUR },
  { "common", TimeSig::COMMON },
  { "cut-common", TimeSig::CUT_COMMON }, // TODO s/b compound glyph
};

bool IsTimeSig(std::string s)
{
  // can be replaced with a star for 'what's the time sig' questions
  Remove(s, '*'); 

  auto it = TIME_SIG_STRS.find(s); 
  return it != TIME_SIG_STRS.end();
}

TimeSig GetTimeSig(std::string s)
{
  // can be replaced with a star for 'what's the time sig' questions
  Remove(s, '*'); 

  auto it = TIME_SIG_STRS.find(s); 
  return it->second;
}

std::string GetStr(TimeSig ts)
{
  for (auto it = TIME_SIG_STRS.begin(); it != TIME_SIG_STRS.end(); ++it)
  {
    if (it->second == ts)
    {
      return it->first;
    }
  }
  return "";
}

std::string LineEnd(bool oneLine)
{
  return (oneLine ? ";" : "\n");
}

// Get GuiMusicScore glyph string from short code
std::string GetStr(std::string s)
{
  bool dot = Contains(s, '.');
  Remove(s, '.');
  bool rest = Contains(s, 'r');
  Remove(s, 'r');

  std::string out = "UNKNOWN";
  if (s == "c") out = "crotchet";
  else if (s == "q") out = "quaver";
  else if (s == "qq") out = "semiquaver";
  else if (s == "m") out = "minim";
  else if (s == "sb") out = "semibreve";

  if (rest)
  {
    out = "rest-" + out;
  }
  else if (s != "sb")
  {
    out += "-up"; // TODO up/down flag
  }

  if (dot)
  {
    // TODO raised dot if glyph is on a line
    out = "dotted-" + out + "-raised-dot";
  }
  return out;
}

void Tie::SetPos()
{
  assert(m_leftGlyph != nullptr);
  assert(m_rightGlyph != nullptr);

  m_leftX  = m_leftGlyph->x;
  m_leftGlyph->SetTieLeft(this);

  m_rightX = m_rightGlyph->x;
  m_rightGlyph->SetTieRight(this);
}

std::string Glyph::TimeBefore() const
{
  std::string res;

  bool yesTime = (timeval > 0);
  if (yesTime)
  { 
    float start = startTime;
    if (start == 0)
    {
      start = 0.01f; // so first glyph is not highlighted until anim starts
    }     
    res += "TIME, " + Str(start) + ", " + Str(timeval + startTime) + " ; ";

    if (!IsRest(realGlyphName) && !m_tieRight)
    {
      // Output MIDI note event, unless on RHS of a tie
      res += "NOTE_ON, " + Str(pitch) + ", " + Str(start) + " ; ";
    }
  }
  return res;
}

std::string Tie::ToString() const
{ 
  // Control points: start and end points in x; y value, depending
  //  on whether the tie is 'n' or 'u' shape. 
  // Inner control points: centre, and one near each end to give
  //  desired shape.
  float y = 1.f; // for 'n' shape -- TODO
  // If U shape
  y = 1.1f; 
  float w = m_rightX - m_leftX;
  const float TIE_ASPECT_RATIO = 8.f;
  float h = w / TIE_ASPECT_RATIO;
  // if u shape
  h = -h;
  float xoff = X_OFFSET_RIGHT * .6f;
  
  const float CP = 0.18f; // control point, for shape
  std::vector<float> coords =
  { 
    m_leftX + xoff,  y, // we need to duplicate the first for spline calc
    m_leftX + xoff,  y,
    Interp(m_leftX, m_rightX, CP) + xoff, y + (h * 0.8f), // give shape
    Interp(m_leftX, m_rightX, .5f) + xoff, y + h, // centre
    Interp(m_leftX, m_rightX, (1.f - CP)) + xoff, y + (h * 0.8f), // give shape
    m_rightX + xoff, y, 
    m_rightX + xoff, y, // TODO do we need this last one?
  };
  
  std::string res = "curve, ";
  int n = coords.size(); 
  for (int i = 0; i < n; i++)
  { 
    res += Str(coords[i] * scale) + (i < (n - 1) ? ", " : "");
  }
  return res;
}

std::string Glyph::TimeAfter() const
{
  std::string res;

  bool yesTime = (timeval > 0);
  if (yesTime)
  { 
    if (!IsRest(realGlyphName) && !m_tieLeft)
    {
      // Output MIDI note off event, unless the note is on LHS of a tie,
      //  in which case it will last longer.
      // Follow chain of ties back to start of tie, to get total length.
      // TODO 
      res += " ; NOTE_OFF, " + Str(pitch) + ", " + Str(timeval + startTime);
    }

    // Cancel time for subsequent glyphs (but postprocess to strip out
    //  unnecessary cancellations)
    res += " ; TIME, -1, -1";
  }
  return res;
}

void MakeScore::Preprocess()
{

//  std::cout << "Preprocessed input: " << m_input << "\n";
}

// Tokenise input string and add each token to internal representation.
void MakeScore::AddTokens()
{
  std::stringstream ss(m_input);

  // Split space-separated input into a vector of strings.
  std::vector<std::string> strs {
    std::istream_iterator<std::string>{ss},
    std::istream_iterator<std::string>{}
  };

  int n = strs.size();
  for (int i = 0; i < n; i++)
  {
    const std::string& s = strs[i];

    if (s == "|")
    {
      Bar* bar = new Bar;
      bar->SetScale(m_scale);
      // Copy time sig over to next bar
      bar->SetTimeSig(m_bars.back()->GetTimeSig());
      // TODO Copy key sig over

      m_bars.push_back(std::unique_ptr<Bar>(bar));
    }
    else if (s == "t") 
    {
      // Tie next 2 notes? Or prev and next - we can get position of prev.
      AddTie();
    }
    else if (IsBeam(s))
    {
      AddBeam(s);
    }
    else if (IsTimeSig(s))
    {
      AddTimeSig(s);
    }
    else 
    {
      AddGlyph(s);
    }
  }
}

void MakeScore::MakeInternal()
{
  // Add first default bar
  Bar* bar = new Bar;
  bar->SetScale(m_scale);
  m_bars.push_back(std::unique_ptr<Bar>(bar));

  // Tokenise input string and add each token to internal representation.
  AddTokens();

  CalcBarSizesAndPositions();

  CalcStartTimes();
}

void MakeScore::CalcStartTimes()
{
  // First, get the total time duration for all bars.
  // Then normalise, and acculumate time values of all glyphs to set the
  //  starting time of each one.

  float totalDuration = 0;
  for (auto& bar : m_bars)
  {
    totalDuration += bar->GetDuration();
  }

  // Use totalDuration to normalise duration of each glyph, and cumulative
  //  duration, so we set the start time of each glyph within 0..1
 
  float acc = 0; 
  for (auto& bar : m_bars)
  {
    acc = bar->CalcNormalisedTimes(totalDuration, acc);  
  }
}

void MakeScore::CalcBarSizesAndPositions()
{
  // Now loop over the bars. From the number of glyphs in each bar,
  //  work out the relative width of each bar.
  // For now, assume only one line.
  int totalNumGlyphs = 0;
  for (auto& bar : m_bars)
  {
    int gc = bar->GetGlyphCount(); 
    totalNumGlyphs += gc;
  }

  // Bar calculates its width as fraction of PAGE_WIDTH
  for (auto& bar : m_bars)
  {
    bar->SetWidth(totalNumGlyphs, PAGE_WIDTH);
  }

  // Set (left, bottom) position of each bar
  float x = 0;
  float y = m_y;

  for (auto& bar : m_bars)
  {
    bar->SetPos(x, y);
    x += bar->GetWidth();
  }

  // Set left and right positions of ties
  for (auto& tie : m_ties) 
  {
    // Look up positions of glyphs the tie connects
    tie->SetPos(); 
  }
}

std::string MakeScore::ToString()
{
  std::string res;

  // First, output a stave. For rhythm only, it's a single line.
  // TODO Multiple lines 
  res += "stave-line, 0, " + Str(DEFAULT_HEIGHT + m_y) + ", " + 
    Str(PAGE_WIDTH) + ", " + Str(m_scale) + 
    LineEnd(m_outputOnOneLine);

  for (auto& b : m_bars)
  {
    res += b->ToString(m_outputOnOneLine);
  }

  for (auto& t : m_ties)
  {
    res += t->ToString();
    res += LineEnd(m_outputOnOneLine);
  }

  return res;
}

void CommandLineParams(int argc, char** argv, MakeScore& ms)
{
  for (int i = 1; i < argc; i++)
  {
    std::string param = argv[i];

    if (param == "--oneline")
    {
      // All on one line
      ms.SetOutputOneLine(true);
    }
  }
}

bool IsQuote(char c)
{
  return c == '"';
}

// Strip " char from beginning and end of string
void StripQuotes(std::string& s)
{
  if (s.size() < 2)
  {
    return;
  }
  if (IsQuote(s[0]))
  {
    s = s.substr(1, s.size() - 2);
  }
}

#ifndef CATCH
// Don't build this main function for unit test exe

int main(int argc, char** argv)
{
  std::string input;
  std::getline(std::cin, input);

  // Chop off whitespace and quotes - needed for Win, not Mac
  Trim(input);
  StripQuotes(input);

std::cout << "// " << input << "\n"; 

  MakeScore ms(input);

  CommandLineParams(argc, argv, ms);

  ms.SetScale(0.6f);

  // For single line rhythm, centre vertically
  // TODO Auto centre 1 or more lines
  ms.SetY(0.5f);

  // TODO Transform input:
  // Add beam groupings
  // Replace beamed quaver/semiquaver glyphs with crotchet glyphs
  ms.Preprocess();

  ms.MakeInternal();

  // Output final string.
  // Don't append a newline char, so we can add more to this line, in
  //  enclosing script.
  std::cout << ms.ToString();

  return 0;
}

#endif // CATCH


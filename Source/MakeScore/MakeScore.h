// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017
//
// * MakeScore *
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

static const int DEFAULT_PITCH = 69; // default pitch = A above middle C

static const float X_OFFSET_RIGHT = 0.25f;

// Time values
using TimeValue = float;
static const float TIMEVAL_SEMIBREVE   = 1.f;
static const float TIMEVAL_MINIM       = TIMEVAL_SEMIBREVE / 2.f;
static const float TIMEVAL_CROTCHET    = TIMEVAL_MINIM / 2.f;
static const float TIMEVAL_QUAVER      = TIMEVAL_CROTCHET / 2.f;
static const float TIMEVAL_SEMIQUAVER  = TIMEVAL_QUAVER / 2.f;

enum class BeamLevel
{
  BEAM_LEVEL_1 = 0,
  BEAM_LEVEL_2 = 1,
};

enum class TimeSig
{
  TWO_FOUR,
  THREE_FOUR,
  FOUR_FOUR,
  COMMON,
  CUT_COMMON
};

std::string GetStr(TimeSig ts);
std::string GetStr(std::string s);
bool Contains(const std::string& s, char c);
std::string Remove(std::string& s, char c);
std::string Str(float f);
float GetHeight(BeamLevel bl);
std::string LineEnd(bool oneLine);
float GetTimeVal(std::string s);
TimeSig GetTimeSig(std::string s);

class MakeScore
{
public:
  MakeScore(const std::string& in) : m_input(in) { }

  std::string ToString();
  void MakeInternal();
  void SetScale(float scale) { m_scale = scale; }
  void SetY(float y) { m_y = y; }

  // Add beam groupings
  // Replace beamed quaver/semiquaver glyphs with crotchet glyphs
  void Preprocess();

  void SetOutputOneLine(bool oneLine)
  {
    m_outputOnOneLine = oneLine;
  }

private:

  void AddTimeSig(const std::string& s)
  {
    m_bars.back()->AddTimeSig(s);
  }

  void AddGlyph(const std::string& s)
  {
    m_bars.back()->AddGlyph(s);

    // If last tie has no right connection, connect it now to the
    //  glyph we just added.
    if (!m_ties.empty())
    {
      Tie* tie = m_ties.back().get();
      if (!tie->IsRhsSet())
      {
        assert(!m_bars.empty());
        tie->SetRightGlyph(m_bars.back()->m_glyphs.back().get());
      }
    }
  }

  void AddBeam(const std::string& s)
  {
    m_bars.back()->AddBeam(s);
  }

  void AddTie()
  {
    // Set bar and position of the left glyph of the tie
    if (m_bars.empty())
    {
      std::cout << "// *** Error, no left glyph for tie to refer to.\n";
      return;
    }
 
    Tie* tie = new Tie;
    tie->SetLeftGlyph(m_bars.back()->m_glyphs.back().get());

    tie->SetScale(m_scale);

    m_ties.push_back(std::unique_ptr<Tie>(tie));
  }

  void AddTokens();
  void CalcBarSizesAndPositions();
  void CalcStartTimes();

private:
  constexpr static const float PAGE_WIDTH = 4.0f;

  // Height of stave line and glyphs on it, relative to bottom of bar lines.
  constexpr static const float DEFAULT_HEIGHT = 0.2f;
  
  // If true, all glyphs on one line, separated by ';'
  // Else each is on a separate line.
  bool m_outputOnOneLine = false;

  // Scale of all glyphs
  float m_scale = 1.0f;

  float m_y = 0;

  std::string m_input;

  struct IGlyph
  {
    virtual ~IGlyph() = default;
    virtual std::string ToString() const = 0;

    void SetScale(float s) { scale = s; }

    float x = 0;
    // Default height is on single middle stave line for rhythm
    float y = DEFAULT_HEIGHT;
    float scale = 1.0f;
  };

  struct Glyph;
  struct Tie : public IGlyph
  {
    virtual std::string ToString() const; 

    void SetPos();

    bool IsRhsSet() const { return m_rightGlyph != nullptr; }

    void SetLeftGlyph(Glyph* left) { m_leftGlyph = left; }
    void SetRightGlyph(Glyph* right) { m_rightGlyph = right; }
 
    // When we create Tie, we only know the glyph on the left end.
    // Later on we know the next note, then we have the right end too.
    Glyph* m_leftGlyph = nullptr;
    Glyph* m_rightGlyph = nullptr;

    // Set left and right x-coords once positions of both glyphs have been
    //  set.
    float m_leftX = 0;
    float m_rightX = 0;
  };

  struct Glyph : public IGlyph
  {
    Glyph() = default;
    Glyph(const std::string& str_, int order_) : 
      displayGlyphName(GetStr(str_)), realGlyphName(str_),
      order(order_) 
    {
      HandleStar();
    }

    void HandleStar()
    {
      if (Contains(realGlyphName, '*'))
      {
        Remove(realGlyphName, '*');
        displayGlyphName = '*';
      }
    }

    std::string TimeBefore() const;
    std::string TimeAfter() const;

    std::string ToString() const override
    {
      // Add special glyphs for timing before and after - this is
      //  for animation and MIDI events. 
      std::string res = TimeBefore();

      res += displayGlyphName + ", " + Str(x) + ", " + Str(y) + 
        ", " + Str(scale) + ", " + Str(scale);
    
      res += TimeAfter();

      return res;
    }

    void SetTimeVal(float timeval_) { timeval = timeval_; }

    void SetDisplayNameForBeamedNote()
    {
      // E.g. "q" or "qq" -> "crotchet-up" for a beamed quaver.
      // Take dottedness into account.

      bool dot = Contains(realGlyphName, '.'); 
      displayGlyphName = GetStr(dot ? "c." : "c");
    }

    // Call to set this glyph as the left hand side of the given tie
    void SetTieLeft(Tie* tie) { m_tieLeft = tie; }

    // Set this glyph as the right hand side of the given tie
    void SetTieRight(Tie* tie) { m_tieRight = tie; }

    int order = 0; // horiz position in bar 

    // Two glyph names. E.g. we have a quaver, but it's drawn using a 
    //  crotchet glyph because it's beamed. So its 'real' name is 'q',
    //  but its display name is 'crotchet'.
    std::string displayGlyphName;
    std::string realGlyphName;

    // Time value for this glyph, i.e. its duration.
    TimeValue timeval = 0;

    // Start time is the accumulated time values of all preceding glyphs.
    TimeValue startTime = 0;

    int pitch = DEFAULT_PITCH; 
 
    // Points to tie - we are the LEFT glyph of the tie
    Tie* m_tieLeft = nullptr;
 
    // Points to tie - we are the RIGHT glyph of the tie
    Tie* m_tieRight = nullptr;
  };

  // Time sigs are always left-aligned, no offset
  struct TimeSigGlyph : public Glyph
  {
    TimeSigGlyph(const std::string& s) : Glyph() 
    { 
      realGlyphName = s;
      displayGlyphName = s;
      y = 0; 
      HandleStar();
    }
  };

  struct Beam : public IGlyph
  {
    Beam() = default;
    Beam(BeamLevel level_, int left_, int right_) : 
      level(level_), left(left_), right(right_) {}

    static constexpr float BEAM_HEIGHT = 0.06f;
    static constexpr float BEAM_Y_MIN = 0.770f;

    std::string ToString() const override
    {
      float ymin = y;
      float xoff = 0;
      if (upNotDown)
      {
        // Gap between beams/flags is half beam height, so y offset is * 1.5
        ymin -= BEAM_HEIGHT * 1.5f * GetHeight(level);
        ymin += BEAM_Y_MIN;
        xoff = X_OFFSET_RIGHT; // move to right as stem is on right of note  
      }
      float ymax = ymin + BEAM_HEIGHT;

      return "quad, " + 
        Str((xmax + xoff) * scale) + ", " + Str(ymin * scale) + ", " + 
        Str((xmax + xoff) * scale) + ", " + Str(ymax * scale) + ", " + 
        Str((xmin + xoff) * scale) + ", " + Str(ymax * scale) + ", " + 
        Str((xmin + xoff) * scale) + ", " + Str(ymin * scale);
    }

    BeamLevel level = BeamLevel::BEAM_LEVEL_1;
    int left = 0;
    int right = 0;
    float xmin = 0;
    float xmax = 0;

    // note type, so we know where to offset the quad corners
    bool upNotDown = true; 
  };

  struct Flag : public Beam
  {
    Flag (BeamLevel level_, int left_, int right_, bool stemDir_) :
      Beam(level_, left_, right_),
      stemLeftNotRight(stemDir_)
    {
    }

    std::string ToString() const override
    {
      float ymin = y;
      float xoff = 0;
      if (upNotDown)
      {
        ymin -= BEAM_HEIGHT * 1.5f * GetHeight(level);
        ymin += BEAM_Y_MIN;
        xoff = X_OFFSET_RIGHT; // move to right as stem is on right of note  
      }
      float ymax = ymin + BEAM_HEIGHT;

      float xmax_ = xmax;
      float xmin_ = xmin;
      const float FLAG_W = 0.2f; // flag is this proportion of full beam
      if (stemLeftNotRight)
      {
        xmax_ = xmin + (xmax - xmin) * FLAG_W;
      }
      else
      {
        xmin_ = xmax - (xmax - xmin) * FLAG_W;      
      }

      return "quad, " + 
        Str((xmax_ + xoff) * scale) + ", " + Str(ymin * scale) + ", " + 
        Str((xmax_ + xoff) * scale) + ", " + Str(ymax * scale) + ", " + 
        Str((xmin_ + xoff) * scale) + ", " + Str(ymax * scale) + ", " + 
        Str((xmin_ + xoff) * scale) + ", " + Str(ymin * scale);
    }

    // Stem to which the flag is attached is on the left (true) or
    //  right (false).
    bool stemLeftNotRight = true;
  };

  struct Bar
  {
    // Sequence of glyphs, left to right, in the bar, following any key sig
    //  and time sig.
    std::vector<std::unique_ptr<Glyph>> m_glyphs;

    // Optional key sig glyph for the bar, at left edge.
    // TODO Handle 'naturalising' key sig before a new key sig, i.e. there
    //  could be up to 2 key sig glyphs. Also, key sigs can be a lot wider
    //  than other glyphs.
    std::unique_ptr<Glyph> m_keySigGlyph;

    // Optional time sig glyph, at left of bar after key sig, if there is one.
    std::unique_ptr<Glyph> m_timeSigGlyph;

    // Beams connecting ordered glyphs
    std::vector<std::unique_ptr<Beam>> m_beams; 

    float m_x = 0;
    float m_y = 0;
    float m_width = 0;
    float m_scale = 1.0f;

    // Time sig: we can use this to check for errors, and do beams/groups
    //  automatically. Also, we can use it to set times for each glyph,
    //  for animation and midi events.
    TimeSig m_timeSig = TimeSig::FOUR_FOUR;

    void SetTimeSig(TimeSig ts)
    {
      m_timeSig = ts;
    }

    TimeSig GetTimeSig() const
    {
      return m_timeSig;
    }

    // Return the total time for the bar.
    // The time val of all the glyph members should add up to this.
    TimeValue GetDuration() const
    {
      switch (m_timeSig)
      {
      case TimeSig::COMMON:
      case TimeSig::FOUR_FOUR:
        return 4.f * TIMEVAL_CROTCHET;

      case TimeSig::THREE_FOUR:
        return 3.f * TIMEVAL_CROTCHET;

      case TimeSig::CUT_COMMON:
      case TimeSig::TWO_FOUR:
        return 2.f * TIMEVAL_CROTCHET;
      }
      return 0; 
    }

    float CalcNormalisedTimes(float totalDuration, float start)
    {
      // Normalise glyph durations, and accumulate to get start times.
      for (auto& g : m_glyphs)
      {
        g->timeval /= totalDuration;
        g->startTime += start;
        start += g->timeval;
      } 
      return start;
    }
 
    void SetScale(float scale)
    {
      m_scale = scale;
    }

    // Get GuiMusicScore glyph string from short code
    static std::string GetStr(std::string s)
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
      else 
      {
        std::cout << "// *** Failed Trying to look up glyph for " << s << "\n";
      }
 
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

    void AddGlyph(const std::string& s)
    {
      float order = static_cast<int>(m_glyphs.size());
     
      Glyph* gl = new Glyph(s, order);
      gl->SetScale(m_scale); 

      // Set duration for this musical symbol
      gl->SetTimeVal(GetTimeVal(s));

      m_glyphs.push_back(std::unique_ptr<Glyph>(gl));
    }

    void AddTimeSig(const std::string& s)
    {
      SetTimeSig(::GetTimeSig(s));

      Glyph* gl = new TimeSigGlyph(s);
      gl->SetScale(m_scale); 
      m_timeSigGlyph = std::unique_ptr<Glyph>(gl);
    }

    void AddBeam(const std::string& s)
    {
      // Beams go between 2 glyphs.
      // Currently we always beam 2 adjacent glyphs, i.e. from n-1 to n
      float n = static_cast<int>(m_glyphs.size());

      // Flags are attached to one glyph, and can go left or right.
      
      // Level is quaver/semi etc
      m_beams.push_back(std::unique_ptr<Beam>(
        new Beam(BeamLevel::BEAM_LEVEL_1, n - 1, n)));

      if (s == "--" || s == "-")
      {
        // Beam 2 quavers, we are done.
      }
      else if (s == "==" || s == "=")
      {
        // 2 semiquavers, add 2nd beam
        m_beams.push_back(std::unique_ptr<Beam>(
          new Beam(BeamLevel::BEAM_LEVEL_2, n - 1, n)));
      }
      else if (s == "-=")
      {
        // Semiq flag, attached to right stem
        m_beams.push_back(std::unique_ptr<Beam>(
          new Flag(BeamLevel::BEAM_LEVEL_2, n - 1, n, false)));
      }
      else if (s == "=-")
      {
        // Semiq flag, attached to left stem
        m_beams.push_back(std::unique_ptr<Beam>(
          new Flag(BeamLevel::BEAM_LEVEL_2, n - 1, n, true)));
      }
    }

    std::string ToString(bool oneLine)
    {
      std::string res;

      // Optional time sig
      if (m_timeSigGlyph)
      {
        res += m_timeSigGlyph->ToString() + LineEnd(oneLine);
      }

      // Set display names for beamed (semi)quavers etc. 
      for (auto& b : m_beams)
      {
        m_glyphs[b->left]->SetDisplayNameForBeamedNote();
        m_glyphs[b->right]->SetDisplayNameForBeamedNote();
      }

      for (auto& g : m_glyphs)
      {
        res += g->ToString() + LineEnd(oneLine);
      }
  
      for (auto& b : m_beams)
      {
        b->SetScale(m_scale); 
        res += b->ToString() + LineEnd(oneLine);
      }
 
      // Bar lines
      res += 
        "bar-line, " + Str(m_x) + ", " + Str(m_y) +
        ", " + Str(m_scale) + ", " + Str(m_scale) + 
        LineEnd(oneLine);
 
      res += 
        "bar-line, " + Str(m_x + m_width) + ", " + Str(m_y) +
        ", " + Str(m_scale) + ", " + Str(m_scale) + 
        LineEnd(oneLine);
 
      return res;
    }

    // Get number of distinct glyphs horizontally
    // (e.g. a chord is only one 'glyph' as all the notes take up only 
    //  one horizontal space/share the same stem)
    int GetGlyphCount() const
    {
      int glyphCount = static_cast<int>(m_glyphs.size());
      if (m_timeSigGlyph)
      {  
        glyphCount++;
      }
      return glyphCount; 
    }

    void SetWidth(int totalNumGlyphs, float pageWidth)
    {
      int glyphCount = GetGlyphCount();

      m_width = static_cast<float>(glyphCount) /
        static_cast<float>(totalNumGlyphs) * pageWidth / m_scale;
    }
 
    float GetWidth() const
    {
      return m_width;
    } 

    // x is the left edge of the bar.
    // From this and the width, we can set the final x-coord of each glyph.
    // y is an offset added to the y-coord of each glyph (all the same for
    //  rhythm scores). 
    void SetPos(float x, float y)
    {
      m_x = x;
      m_y = y;

      // xoff is distance from left bar line to first glyph, and also distance
      //  from last glyph to right bar line.
      float numGlyphs = static_cast<float>(GetGlyphCount());
      float xoff = m_width / (numGlyphs + 1.0f);
      float w = 0;
      if (numGlyphs > 1)
      {
        w = (m_width - 2 * xoff) / (numGlyphs - 1.0f);
        if (m_timeSigGlyph)
        {
          const float TIME_SIG_WIDTH = 0.3f; // ?
          // Reduce available width
          if (numGlyphs > 2)
          {
            w = (m_width - TIME_SIG_WIDTH - 2 * xoff) / (numGlyphs - 2.0f);
          }
          xoff += TIME_SIG_WIDTH; // move other glyphs to the right      
 
          m_timeSigGlyph->x = x; // plus some extra?
          m_timeSigGlyph->y += y;
        }
      }

      // Set coord of each glyph
      // Compensate for glyph width, move to the left a bit
      // TODO depends on glyph type?, e.g. semibreve is slightly wider.
      float xfudge = -0.2f; 

      for (auto& g : m_glyphs)
      {
        g->x = x + w * static_cast<float>(g->order) + xoff + xfudge;

        g->y += y; // TODO Whether or not this is correct will become
                   //  apparent when we have multi-line scores.
      }

      // Set position of beam left and right ends
      for (auto& b : m_beams)
      {
        b->xmin = x + w * static_cast<float>(b->left)  + xoff + xfudge;
        b->xmax = x + w * static_cast<float>(b->right) + xoff + xfudge;
        b->y += y;
      }
    }
  };

  // Ordered sequence of bars in the score.
  std::vector<std::unique_ptr<Bar>> m_bars;

  // Ties connect glyphs which can be in different bars, so ties are not
  //  per-bar.
  std::vector<std::unique_ptr<Tie>> m_ties;

};

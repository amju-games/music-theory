// Sub project to convert easily-authorable music content
//  into list of glyphs.
// clang++ MakeScore.cpp -std=c++11

// Music score coords:
//  +--------------------------+   0.4
//  +--------------------------+
//  +--------------------------+
//  +--------------------------+
//  +--------------------------+   0.0
// 0.0                     PAGE_WIDTH

#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// Convenience functions
bool Contains(const std::string& s, char c)
{
  return std::find(s.begin(), s.end(), c) != s.end();
}

std::string Remove(std::string& s, char c)
{
  s.erase(std::remove(s.begin(), s.end(), c), s.end()); 
  return s;
}

bool IsBeam(const std::string& s)
{
  return Contains(s, '-') || Contains(s, '=');
}

enum class TimeSig
{
  TWO_FOUR,
  THREE_FOUR,
  FOUR_FOUR,
  COMMON,
  CUT_COMMON
};

static const std::map<std::string, TimeSig> TIME_SIG_STRS = 
{
  { "2/4", TimeSig::TWO_FOUR },
  { "3/4", TimeSig::THREE_FOUR },
  { "4/4", TimeSig::FOUR_FOUR },
  { "common", TimeSig::COMMON },
  { "cut-common", TimeSig::CUT_COMMON }, // TODO s/b compound glyph
};

bool IsTimeSig(const std::string& s)
{
  auto it = TIME_SIG_STRS.find(s); 
  return it != TIME_SIG_STRS.end();
}

TimeSig GetTimeSig(const std::string& s)
{
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
  return (oneLine ? "; " : "\n");
}

class MakeScore
{
public:
  MakeScore(const std::string& in) : m_input(in) { }

  std::string ToString();
  void MakeInternal();
  void SetScale(float scale) { m_scale = scale; }

  // Add beam groupings
  // Replace beamed quaver/semiquaver glyphs with crotchet glyphs
  void Preprocess();

private:

  void AddTimeSig(const std::string& s)
  {
    m_bars.back()->AddTimeSig(s);
  }

  void AddGlyph(const std::string& s)
  {
    m_bars.back()->AddGlyph(s);
  }

  void AddBeam(const std::string& s)
  {
    m_bars.back()->AddBeam(s);
  }

private:
  constexpr static const float PAGE_WIDTH = 4.0f;

  constexpr static const float DEFAULT_HEIGHT = 0.2f;
  
  // If true, all glyphs on one line, separated by ';'
  // Else each is on a separate line.
  bool m_outputOnOneLine = false;

  // Scale of all glyphs
  float m_scale = 1.0f;

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

  struct Glyph : public IGlyph
  {
    Glyph() = default;
    Glyph(const std::string& str_, int order_) : order(order_), str(str_) {}

    std::string ToString() const override
    {
      // x-order for now, position after final pass allocates positions.
      return str + ", " + std::to_string(x) + ", " + std::to_string(y) + 
        ", " + std::to_string(scale) + ", " + std::to_string(scale);
    }

    int order = 0; // horiz position in bar 
    std::string str;
  };

  // Time sigs are always left-aligned, no offset
  struct TimeSigGlyph : public Glyph
  {
    TimeSigGlyph(const std::string& s) : Glyph(s, 0) { y = 0; }
  };

  struct Beam : public IGlyph
  {
    Beam() = default;
    Beam(int left_, int right_) : left(left_), right(right_) {}

    std::string ToString() const override
    {
      return "Beam, " + std::to_string(left) + " to " + std::to_string(right);
    }

    int left = 0;
    int right = 0;
  };

  struct Bar
  {
    // Sequence of glyphs, left to right, in the bar.
    std::vector<std::unique_ptr<Glyph>> m_glyphs;

    // Beams connecting ordered glyphs; in fact this  could be anything where 
    //  order is not important, e.g. ties too.
    std::vector<std::unique_ptr<IGlyph>> m_beams; 

    float m_x = 0;
    float m_y = 0;
    float m_width = 0;
    float m_scale = 1.0f;

    // Time sig: we can use this to check for errors, and do beams/groups
    //  automatically.
    TimeSig m_timeSig = TimeSig::FOUR_FOUR;

    void SetTimeSig(TimeSig ts)
    {
      m_timeSig = ts;
    }

    TimeSig GetTimeSig() const
    {
      return m_timeSig;
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
        out = "dotted-" + out;
      }
      return out;
    }

    void AddGlyph(const std::string& s)
    {
      float order = static_cast<int>(m_glyphs.size());
     
      Glyph* gl = new Glyph(GetStr(s), order);
      gl->SetScale(m_scale); 
      m_glyphs.push_back(std::unique_ptr<Glyph>(gl));
    }

    void AddTimeSig(const std::string& s)
    {
      SetTimeSig(::GetTimeSig(s));

      Glyph* gl = new TimeSigGlyph(s);
      gl->SetScale(m_scale); 
      m_glyphs.push_back(std::unique_ptr<Glyph>(gl));
    }

    void AddBeam(const std::string& s)
    {
      // Beams go between 2 glyphs.
      // Currently we always beam 2 adjacent glyphs, i.e. from n-1 to n
      float n = static_cast<int>(m_glyphs.size());

      // Flags are attached to one glyph, and can go left or right.
      
      // TODO level, i.e. quaver/semi etc
      m_beams.push_back(std::unique_ptr<IGlyph>(new Beam(n - 1, n)));

      if (s == "--" || s == "-")
      {
        // Beam 2 quavers, we are done.
      }
      else if (s == "==" || s == "=")
      {
        // 2 semiquavers, add 2nd beam
        // TODO level, i.e. quaver/semi etc
        m_beams.push_back(std::unique_ptr<IGlyph>(new Beam(n - 1, n)));
      }
      else if (s == "-=")
      {
        // Semiq flag, on left of stem
      }
      else if (s == "=-")
      {
        // Semiq flag, on right of stem
      }
    }

    std::string ToString(bool oneLine)
    {
      std::string res;

      for (auto& g : m_glyphs)
      {
        res += g->ToString() + LineEnd(oneLine);
      }
  
      for (auto& g : m_beams)
      {
        res += g->ToString() + LineEnd(oneLine);
      }
 
      // Bar lines
      res += 
        "bar-line, " + std::to_string(m_x) + ", " + std::to_string(m_y) +
        ", " + std::to_string(m_scale) + ", " + std::to_string(m_scale) + 
        LineEnd(oneLine);
 
      res += 
        "bar-line, " + std::to_string(m_x + m_width) + ", " + std::to_string(m_y) +
        ", " + std::to_string(m_scale) + ", " + std::to_string(m_scale) + 
        LineEnd(oneLine);
 
      return res;
    }

    // Get number of distinct glyphs horizontally
    // (e.g. a chord is only one 'glyph' as all the notes take up only 
    //  one horizontal space)
    int GetGlyphCount() const
    {
      return static_cast<int>(m_glyphs.size());
    }
  
    void SetWidth(int totalNumGlyphs, float pageWidth)
    {
      m_width = static_cast<float>(GetGlyphCount()) /
        static_cast<float>(totalNumGlyphs) * pageWidth / m_scale;

      // Now we can set the x-coord of each glyph in the bar (relative to
      //  bar start)
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
      const float xoff = m_width / (numGlyphs + 1.0f);
      float w = 0;
      if (numGlyphs > 1)
      {
        w = (m_width - 2 * xoff) / (numGlyphs - 1.0f);
      }

std::cout << "Num glyphs: " << numGlyphs << "\n";
std::cout << "xoff: " << xoff << "\n";
std::cout << "w:    " << w << "\n";

      // Set coord of each glyph
      // Compensate for glyph width, move to the left a bit
      // TODO depends on glyph type, e.g. semibreve is slightly wider.
      float xfudge = -0.2f; //? * m_scale;

      for (auto& g : m_glyphs)
      {
        g->x = x + w * static_cast<float>(g->order) + xoff + xfudge;

        g->y += y; // TODO Whether or not this is correct will become
                   //  apparent when we have multi-line scores.
      }
    }
  };

  std::vector<std::unique_ptr<Bar>> m_bars;

};

void MakeScore::Preprocess()
{

  std::cout << "Preprocessed input: " << m_input << "\n";
}

void MakeScore::MakeInternal()
{
  std::stringstream ss(m_input);

  std::vector<std::string> strs {
    std::istream_iterator<std::string>{ss},
    std::istream_iterator<std::string>{}
  };

  // Add first default bar
  Bar* bar = new Bar;
  bar->SetScale(m_scale);
  m_bars.push_back(std::unique_ptr<Bar>(bar));

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

  // Now loop over the bars. From the number of glyphs in each bar,
  //  work out the relative width of each bar.
  // For now, assume only one line.
  int totalNumGlyphs = 0;
  for (auto& bar : m_bars)
  {
    totalNumGlyphs += bar->GetGlyphCount(); 
  }

  // Bar calculates its width as fraction of PAGE_WIDTH
  for (auto& bar : m_bars)
  {
    bar->SetWidth(totalNumGlyphs, PAGE_WIDTH);
  }

  // Set (left, bottom) position of each bar
  float x = 0;
  float y = 0;

  int i = 1;
  for (auto& bar : m_bars)
  {
    std::cout << "// Bar " << i << "\n";
    i++;

    bar->SetPos(x, y);
    x += bar->GetWidth();
  }
}

std::string MakeScore::ToString()
{
  std::string res;

  for (auto& b : m_bars)
  {
    res += b->ToString(m_outputOnOneLine);
  }

  // TODO Num lines 
  res += "stave-line, 0, " + std::to_string(DEFAULT_HEIGHT) + ", " + 
    std::to_string(PAGE_WIDTH) + ", " + std::to_string(m_scale) + 
    LineEnd(m_outputOnOneLine);

  return res;
}

int main()
{
  //std::string input = "c | c c";
  std::string input = "4/4 c c | q -- q cr q. -= qq c";
//  std::getline(std::cin, input);

  std::cout << "Input: \"" << input << "\"\n";

  MakeScore ms(input);
  ms.SetScale(0.5f);

  // TODO Transform input:
  // Add beam groupings
  // Replace beamed quaver/semiquaver glyphs with crotchet glyphs
  ms.Preprocess();

  ms.MakeInternal();

  // TODO
  // Each bar has a default width, and a min and max. Expand/contract
  //  adjacent bars.
  
  std::cout << ms.ToString() << "\n";

  return 0;
}


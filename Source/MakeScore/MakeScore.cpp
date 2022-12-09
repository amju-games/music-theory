// Sub project to convert easily-authorable music content
//  into list of glyphs.

#include <algorithm>
#include <iostream>
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

class MakeScore
{
public:
  MakeScore(const std::string& in) : m_input(in) { }

  std::string ToString();
  void MakeInternal();

private:

  void AddGlyph(const std::string& s)
  {
    m_bars.back().AddGlyph(s);
  }

  void AddBeam(const std::string& s)
  {
    m_bars.back().AddBeam(s);
  }

private:
  std::string m_input;

  struct IGlyph
  {
    virtual ~IGlyph() = default;
    virtual std::string ToString() const = 0;
  };

  struct Glyph : public IGlyph
  {
    Glyph() = default;
    Glyph(const std::string& str_, int order_) : order(order_), str(str_) {}

    std::string ToString() const override
    {
      // x-order for now, position after final pass allocates positions.
      return "\n\t" + str + ", " + std::to_string(order);
      //std::to_string(x) + ", " + std::to_string(y);
    }

    int order = 0; // position in bar 
    float x = 0;
    float y = 0;
    std::string str;
  };

  struct Beam : public IGlyph
  {
    Beam() = default;
    Beam(int left_, int right_) : left(left_), right(right_) {}

    std::string ToString() const override
    {
      return "\n\tBeam, " + std::to_string(left) + " to " + std::to_string(right);
    }

    int left = 0;
    int right = 0;
  };

  struct Bar
  {
    std::vector<std::unique_ptr<IGlyph>> m_glyphs;

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
        out = "dotted-" + out;
      }
      return out;
    }

    void AddGlyph(const std::string& s)
    {
      float order = static_cast<int>(m_glyphs.size());
      
      m_glyphs.push_back(std::unique_ptr<IGlyph>(new Glyph(GetStr(s), order)));
    }

    void AddBeam(const std::string& s)
    {
      // Beams go between 2 glyphs.
      // Currently we always beam 2 adjacent glyphs, i.e. from n-1 to n
      float n = static_cast<int>(m_glyphs.size());

      // Flags are attached to one glyph, and can go left or right.
      
      // TODO level, i.e. quaver/semi etc
      m_glyphs.push_back(std::unique_ptr<IGlyph>(new Beam(n - 1, n)));

      if (s == "--" || s == "-")
      {
        // Beam 2 quavers, we are done.
      }
      else if (s == "==" || s == "=")
      {
        // 2 semiquavers, add 2nd beam
        // TODO level, i.e. quaver/semi etc
        m_glyphs.push_back(std::unique_ptr<IGlyph>(new Beam(n - 1, n)));
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

    std::string ToString()
    {
      std::string res;

      for (auto& g : m_glyphs)
      {
        res += g->ToString() + "; ";
      }
  
      return res;
    }
  };

  std::vector<Bar> m_bars;
};

void MakeScore::MakeInternal()
{
  std::stringstream ss(m_input);

  std::vector<std::string> strs {
    std::istream_iterator<std::string>{ss},
    std::istream_iterator<std::string>{}
  };

  // Add first default bar
  m_bars.push_back(Bar());

  int n = strs.size();
  for (int i = 0; i < n; i++)
  {
    const std::string& s = strs[i];

    if (s == "|")
    {
      m_bars.push_back(Bar());
    }
    else if (IsBeam(s))
    {
      AddBeam(s);
    }
    else 
    {
      AddGlyph(s);
    }
  }
}

std::string MakeScore::ToString()
{
  std::string res;

  int i = 1;
  for (auto& b : m_bars)
  {
    res += "\nBar " + std::to_string(i) + ":\n";
    i++;
    res += b.ToString();
  }

  return res;
}

int main()
{
  std::string input = "m c c | q -- q cr q. -= qq c";
//  std::getline(std::cin, input);

  std::cout << "Input: \"" << input << "\"\n";

  // TODO Transform input:
  // Add beam groupings
  // Replace beamed quaver/semiquaver glyphs with crotchet glyphs

  MakeScore ms(input);
  ms.MakeInternal();

  // TODO Set x coords according to number of glyphs in each bar.
  // Each bar has a default width, and a min and max. Expand/contract
  //  adjacent bars.
  
  std::cout << ms.ToString() << "\n";

  return 0;
}


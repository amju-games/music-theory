#include <iostream>
#include <Timer.h>
#include "GuiScrollScore.h"

namespace Amju
{
const char* GuiScrollScore::NAME = "scroll-score";

bool GuiScrollScore::Load(File* f)
{
  if (!GuiMusicScore::Load(f))
  {
    return false;
  }
  BuildBeatTable(m_beatTable);
  return true;
}

void GuiScrollScore::Update()
{
  GuiMusicScore::Update();

  auto pos = GetLocalPos();
  const float scale = GetSize().x * GetCombinedScale().x;
  pos.x = - m_currentX * scale; // I'm thinking of the next pos as positive x but we need to go backwards
  SetLocalPos(pos);
}

void GuiScrollScore::Animate(float animValue) 
{
  GuiMusicScore::Animate(animValue);

  // Find the next entry in the Beat Table 
  auto it = m_beatTable.lower_bound(animValue);
  if (it == m_beatTable.begin())
  {
    // we are at the start of the piece.
    return;
  }
  else
  {
    --it;
  }

  if (it == m_beatTable.end())
  {
    m_scrollSpeed = 0;
    // Have we reached the end of the piece?
//std::cout << "End of the piece?\n";
  }
  else
  {
    const auto& [time, x] = *it;
    if (time > m_nextT)
    {
      // We have reached the beat. Recalc velocity to reach the next one.
    
std::cout << "Current: " << animValue << " Next time: " << time 
  << " current x: " << m_currentX  << " desired x: " << x << "\n";

      m_currentX = x; 
      m_nextT = time;

/*
      // Find the next time
      ++it;
      if (it == m_beatTable.end())
      {
        m_scrollSpeed = 0;
      }
      else
      {
        const auto& [nextTime, nextX] = *it;
        m_nextT = nextTime;

        // We want to go from current x to new x in (time - animValue) normalized time.
        // We need to convert the time diff into real time from normalized time.
        float dt = nextTime - time;
        Assert(dt >= 0);
        Assert(dt <= 1.f);
        // We need to know the legth of the piece in seconds
        // OR the bpm and the number of beats (we could get this from time sigs and bars?)
        float pieceLength = 10.f; // TODO TEMP TEST
        float actualTime = dt * pieceLength;
        // We need a current x offset
        float dx = x - m_currentX;
        m_currentX = x; 
        //m_scrollSpeed = dx / actualTime;
std::cout << "New speed: " << m_scrollSpeed << " units/sec\n";
      }
*/
    }
  }

  // Scroll from right to left
  const float dt = TheTimer::Instance()->GetDt();
  m_currentX += dt * m_scrollSpeed; 
}

void GuiScrollScore::OnResetAnimation() 
{
  GuiMusicScore::OnResetAnimation();

  m_nextT = 0;
  m_currentX = 0;
  m_scrollSpeed = 0;
}

void GuiScrollScore::BuildBeatTable(BeatTable& beatTable) 
{
  for (const auto& g : m_glyphs)
  {
    const float x = g.m_pos.x;
    const float t = g.m_timeMinMax.x; // Start time for this glyph

//std::cout << t << "\t" << x << "\t'" << g.m_char << "'\n";
    if (g.m_char < ' ')
    {
      continue; // non-printable glyph code, e.g. for a quad, which doesn't use m_pos.
    }

    if (t < 0)
    {
      continue; // we don't care about this glyph if time not set
    }

    // Unfortunately I don't think we can rely on the useful operator[] adding a 
    //  value of zero if the key doesn't exist yet. X coords could be negative, no?
    if (!beatTable.contains(t))
    {
      beatTable[t] = x;
    }
    else
    {
      float& minXSoFar = beatTable[t]; 
      if (x < minXSoFar)
      {
         minXSoFar = x;
      }
    }
  }

  // TODO TEMP TEST: Print the beat table. This shows us normalized times, and
  //  the x-position corresponding to that time.
  for (const auto& [time, x] : beatTable)
  {
    std::cout << "Time " << time << "\tX " << x << "\n";
  }
}
}


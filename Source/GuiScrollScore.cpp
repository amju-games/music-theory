#include <iostream>
#include <GuiPoly.h>
#include <Timer.h>
#include "GuiScrollScore.h"

namespace Amju
{
const char* GuiScrollScore::NAME = "scroll-score";

bool GuiScrollScore::LoadMusicScore(File* f)
{
  if (!GuiMusicScore::LoadMusicScore(f))
  {
    return false;
  }
  BuildBeatTable(m_beatTable);
  AddBeatLines();
  return true;
}

bool GuiScrollScore::Load(File* f)
{
  if (!GuiMusicScore::Load(f))
  {
    return false;
  }
  BuildBeatTable(m_beatTable);
  AddBeatLines();
  return true;
}

void GuiScrollScore::StartCountIn(
  int numCountInBeats, std::function<void()> onFinished)
{
  if (m_beatTable.empty())
  {
std::cout << "Can't count in, no music events!\n";
    return;
  }

  m_countInFinishedFunc = onFinished;

  // Duration of count-in
  m_countInTimeRemaining = static_cast<float>(numCountInBeats) / GetBpm() * 60.f;

  // Get distance to first music event in score. 
  float d = m_beatTable.begin()->second;

  m_countInSpeed = d / m_countInTimeRemaining;

std::cout << "Count in..... time remaining: " << m_countInTimeRemaining 
  << " dist: " << d 
  << "\n";
}

void GuiScrollScore::Update()
{
  // Sigh, can't update here because count-in won't work.
}

void GuiScrollScore::Draw()
{
  // Mini-update in Draw :( ... 
  // We want to update scroll pos when not animating,
  //  when unfortunately Update will not be called.

  // If counting in, scroll without animating
  if (m_countInSpeed > 0)
  {
    const float dt = TheTimer::Instance()->GetDt();
    m_currentX += dt * m_countInSpeed;
 
    m_countInTimeRemaining -= dt;
    if (m_countInTimeRemaining <= 0)
    {
      // Notify client that count in has finished
      m_countInFinishedFunc();

      m_countInSpeed = 0;
      m_countInTimeRemaining = 0;
    }
  }

  auto pos = GetLocalPos();
  const float scale = GetSize().x * GetCombinedScale().x;

  // We want increasingly negative x to scroll R -> L
  pos.x = - m_currentX * scale; 

  SetLocalPos(pos);

  GuiMusicScore::Draw();
}

void GuiScrollScore::Animate(float animValue) 
{
// We don't want to do this, but if we did, we would need to not overwrite
//  the colour of glyphs.

//  GuiMusicScore::Animate(animValue);

  m_animTime = animValue; 

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
/*    
std::cout << "t: " << animValue 
  << " reached: " << time 
  << " old x: " << m_currentX  
  << " new x: " << x; 
*/
      m_currentX = x; 
      m_nextT = time;

      // Find the next time
      ++it;
      if (it == m_beatTable.end())
      {
        //m_scrollSpeed = 0; // Keep scrolling
std::cout << " - end?\n";
      }
      else
      {
        const auto& [nextTime, nextX] = *it;

        // We want to go from current x to next x in (nextTime - time) normalized time.
        float dt = nextTime - time;
        Assert(dt >= 0);
        Assert(dt <= 1.f);
        float dx = nextX - m_currentX;
        m_scrollSpeed = dx / dt;
/*
std::cout << " nextT: " << nextTime 
  << " nextX: " << nextX
  << " vel: " << m_scrollSpeed << " units/sec\n";
*/
      }
    }
  }

  // Scroll from right to left
  static float prevAnimValue = animValue;
  float dt = animValue - prevAnimValue;
  prevAnimValue = animValue;
  m_currentX += m_scrollSpeed * dt; 
}

void GuiScrollScore::OnResetAnimation() 
{
  // Hmm, do we want to call this?
  GuiMusicScore::OnResetAnimation();

  m_animTime = 0; 

  m_nextT = 0;
  m_currentX = 0;
  m_scrollSpeed = 0;
}

void GuiScrollScore::BuildBeatTable(BeatTable& beatTable) 
{
  beatTable.clear();

  for (const auto& g : m_glyphs)
  {
    const float x = g.m_pos.x;
    const float t = g.m_timeMinMax.x; // Start time for this glyph

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

/*
  // TODO TEMP TEST: Print the beat table. This shows us normalized times, and
  //  the x-position corresponding to that time.
  for (const auto& [time, x] : beatTable)
  {
    std::cout << "Time " << time << "\tX " << x << "\n";
  }
*/
}

void GuiScrollScore::AddBeatLines()
{
  // Add lines to show beat table x values
  for (const auto& [time, x] : m_beatTable)
  {
std::cout << "Adding a beat line at x = " << x << "\n";

    auto poly = new GuiPoly;
    poly->SetLocalPos({0, 0});
    poly->AddControlPoint({x, 1.f});
    poly->AddControlPoint({x, -1.f});
    poly->OnControlPointsChanged();
    poly->SetFilledColour(*FromHexString("ff8080"));
    poly->SetOutlineColour(*FromHexString("ff8080"));
    poly->SetStyle(GuiPoly::Style::OUTLINE);
    m_children.push_back(poly);
    m_children.push_back(new GuiFlush);
  }
}

float GuiScrollScore::GetAnimTime() const
{ 
  return m_animTime;
}

const BeatTable& GuiScrollScore::GetBeatTable() const
{
  return m_beatTable;
}
}


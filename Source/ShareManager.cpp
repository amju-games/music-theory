// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Screen.h>
#include <Timer.h>
#include <Unproject.h>
#include "Consts.h"
#ifdef AMJU_IOS
#include "iOSShare.h"
#endif
//#include "MySceneGraph.h"
#include "ROConfig.h"
//#include "Screenshot.h"
#include "ShareManager.h"

namespace Amju
{
ShareManager::ShareManager()
{
  SetShareTextToDefault();
}

ShareManager::~ShareManager()
{
  ClearScreenshotData();
}

void ShareManager::ShareTextAndScreenshot()
{
#ifdef AMJU_IOS
  iOSShareTextAndScreenshot(m_data, m_text, m_w, m_h);
#endif
}

void ShareManager::ClearScreenshotData()
{
  delete [] m_data; // Was allocated on heap with new[]
  m_data = nullptr;
}

//void ShareManager::SetTakeScreenshot(const Vec3f& pos3d, float timeDelay)
//{
//  m_pos3d = pos3d;
//  m_yesTakeScreenshot = true; // TODO countdown number of frames
//  m_x = 0; // calc when we take screenshot by projecting pos3d
//  m_y = 0;
//  m_w = SCREENSHOT_W;
//  m_h = SCREENSHOT_H;
//  m_timer = timeDelay;
//}

//void ShareManager::TakeScreenshot()
//{
//  if (m_yesTakeScreenshot)
//  {
//    m_timer -= TheTimer::Instance()->GetDt();
//    if (m_timer <= 0)
//    {
//      // Delete old screenshot data
//      delete [] m_data; // Was allocated on heap with new[]
//      
//      // Project 3d pos to get screen space centre of screenshot
//      /*
//      PSceneNodeCamera cam = GetGameSceneGraph()->GetCamera();
//      Matrix m = cam->GetMatrix(); // camera look at matrix
//      Matrix p;
//      p.Projection();
//      m = m * p;
//      Vec3f t = m_pos3d * m; // transformed pos
//      */
//      Vec2f pos2d(0, 0); ////t.x, t.y);
//      
//      ////Project(m_pos3d, &pos2d);
//      
//      // Pos2d should be in -1..1 coord space
//      m_x = (pos2d.x * 0.5f + 0.5f) * Screen::X();
//      // TODO y-coord could be from top rather than bottom etc
//      m_y = (pos2d.y * 0.5f + 0.5f) * Screen::Y();
//      
//      // Take the screenshot, data is allocated with new[]
//      m_data = SaveScreenshotToArrayOnHeap(Vec2f(m_x, m_y), Vec2f(m_w, m_h));
//      
//      // http://stackoverflow.com/questions/1352864/how-to-get-uiimage-from-eaglview
//      // gl renders "upside down" so swap top to bottom into new array.
//      // there's gotta be a better way, but this works.
//      int myDataLength = m_w * m_h * 3;
//      unsigned char* buffer2 = new unsigned char[myDataLength];
//      for(int y = 0; y < m_h; y++)
//      {
//        memcpy(buffer2 + (m_h - 1 - y) * m_w * 3, m_data + (y * 3 * m_w), m_w * 3 );
//      }
//      delete [] m_data;
//      m_data = buffer2;
//      
//      m_yesTakeScreenshot = false;
//    }
//  }
//}

void ShareManager::SetShareText(const std::string& text)
{
  m_text = text;
}

void ShareManager::SetShareTextToDefault()
{
  m_text = DEFAULT_SHARE_TEXT;
}
}


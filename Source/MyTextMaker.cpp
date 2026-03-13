#include <AmjuGL.h>
#include <Colour.h>
#include <Timer.h>
#include <StringsFile.h>
#include "MyTextMaker.h"

namespace Amju
{
class MyCharacterDecorator : public SceneNode
{
public:
  MyCharacterDecorator(const Colour& colour, float theta);
  virtual void BeforeDraw();
  virtual void AfterDraw();

private:
  Colour m_colour;
  float m_theta;
};

MyCharacterDecorator::MyCharacterDecorator(const Colour& colour, float theta) :
  m_colour(colour), m_theta(theta)
{
}

void MyCharacterDecorator::BeforeDraw()
{
  AmjuGL::PushMatrix();
  PushColour();
  MultColour(m_colour);
  float s = sin(m_theta);

  float dt = TheTimer::Instance()->GetDt();
  m_theta += dt;
  AmjuGL::Translate(0, 0, s);
}

void MyCharacterDecorator::AfterDraw()
{
  PopColour();
  AmjuGL::PopMatrix();
}

MyTextMaker::MyTextMaker()
{
  SetDataDir("font3d/");
  m_theta = 0;
}

PSceneNode MyTextMaker::Decorate(PSceneNode p)
{
  static Strings strings;
  static bool first = true;
  if (first)
  {
    first = false;
    if (!LoadStrings("colours.txt", &strings))
    {
      std::cout << "Failed to load colours :(\n";
    }
  }
//  static const int MAX_COLOUR = 4;
//  Colour COLOURS[MAX_COLOUR] = 
//  {
//    Colour(1.0f, 0.0f, 0.5f, 1.0f),
//    Colour(0.8f, 0.0f, 0.6f, 1.0f),
//    Colour(1.0f, 0.3f, 0.4f, 1.0f),
//    Colour(0.8f, 0.2f, 0.6f, 1.0f)
//  };

  // Choose colour
  static int c = 0;
  m_colour = FromHexString(strings[c]).value(); // deref optional
  c++;
  if (c == strings.size())
  {
    c = 0;
  }
  SceneNode* decorator = new MyCharacterDecorator(m_colour, m_theta);
  // Get next value for theta
  m_theta += 0.2f; 

  p->SetIsLit(false); //true);
  decorator->AddChild(p);
  decorator->CalcBoundingVol();

  return decorator;
}
}

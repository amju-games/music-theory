#include <GameObjectFactory.h>
#include <DegRad.h>
#include "Bird.h"
#include "AIFly.h"

namespace Amju
{
const char* Bird::NAME = "bird";

GameObject* CreateBird() { return new Bird; }

#ifdef GLOBAL_INIT
static bool reg = TheGameObjectFactory::Instance()->Add(Bird::NAME, &CreateBird);
#endif

static const float XSIZE = 15.0f;
static const float YSIZE = 20.0f;

Bird::Bird()
{
  //m_aabbExtents = Vec3f(XSIZE, YSIZE, XSIZE);
  //m_extentsSet = true;

  AddAI(new AIFly);
}

const char* Bird::GetTypeName() const
{
  return NAME;
}

void Bird::Update()
{
  SetAI(GetAI(AIFly::NAME)); 
  if (m_ai)
  {
//    Player* p = Player::GetPlayer(AMJU_P1);
//    Assert(p);
//    m_ai->SetTarget(p);
    m_ai->Update();
  }

  // Not
  //  Npc::Update();
  // .. because doesn't fall!

//  OnFloorCharacter::Update();
 
  Matrix scale;
  const float BIRD_SCALE = 0.25f;
  scale.Scale(BIRD_SCALE, BIRD_SCALE, BIRD_SCALE); 
  Matrix mat;
  mat.RotateY(DegToRad(m_dirCurrent));
  mat = scale * mat;
  mat.TranslateKeepRotation(m_pos);
  GetSceneNode()->SetLocalTransform(mat);

//  RecalcAABB();
}

}

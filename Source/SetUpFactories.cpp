#include <GuiFactory.h>
#include <SceneNodeFactory.h>
#include "AnimalFactory.h"
#include "BlinkSceneNode.h"
#include "Gui3dScene.h"
#include "GuiDecCurvedPath.h"
#include "GuiMusic2dKeyboard.h"
#include "GuiMusicScore.h"
#include "GuiPatch.h"
#include "GuiScrollScore.h"
#include "GuiTextFile.h"
#include "Md2SceneNode.h"
#include "ParticleFx.h"
#include "SceneNodeGui.h"
#include "SceneTimeline.h"

namespace Amju
{
template <class T>
static void AddToGuiFactory()
{
  TheGuiFactory::Instance()->Add(T::NAME, []()->GuiElement* { return new T; }); 
}

template <class T>
static void AddToSceneNodeFactory()
{
  TheSceneNodeFactory::Instance()->Add(T::NAME, 
    []()->SceneNode* { return new T; }); 
}

template <class T>
static void AddToTimelineFactory()
{
  TheTimelineEventFactory::Instance()->Add(T::NAME, 
    []()->TimelineEvent* { return new T; }); 
}

void SetUpFactories()
{
  // Add game-specific types to Gui factory
  AddToGuiFactory<Gui3dScene>(); // TODO Promote to amjulib
  AddToGuiFactory<GuiDecCurvedPath>(); // TODO Promote to amjulib
  AddToGuiFactory<GuiMusic2dKeyboard>();
  AddToGuiFactory<GuiMusicScore>();
  AddToGuiFactory<GuiPatch>(); // TODO Promote to amjulib
  AddToGuiFactory<GuiScrollScore>();
  AddToGuiFactory<GuiTextFile>(); // TODO Promote to amjulib

  //Add game-specific types to Scene node factory
  // TODO These are not game specific! Add to amjulib!!
  ParticleFx::AddToFactory();

  AddToSceneNodeFactory<SceneNodeGui>(); // TODO Promote to amjulib
  AddToSceneNodeFactory<Md2SceneNode>(); // TODO Promote to amjulib
  AddToSceneNodeFactory<Md2SceneNodeWith1Texture>(); // TODO Promote to amjulib? I think?
  AddToSceneNodeFactory<BlinkSceneNode>(); // TODO Promote to amjulib

  // Timeline types (cut scene anims)
  AddToSceneNodeFactory<SceneTimeline>(); // TODO Promote to amjulib
  AddToSceneNodeFactory<SceneTimelineAppend>(); 
  AddToTimelineFactory<EventSetAnim>(); // TODO Promote

  SetUpAnimalFactory();
}
}


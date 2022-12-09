// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include <GuiElement.h>
#include <RCPtr.h>
#include <SceneNode.h>
#include <Vec3.h>

namespace Amju
{
class File;

// * Tappable *
// Tappable items in the main menu corridor - user can tap on these items,
//  and is presented with more info, etc.
class Tappable : public RefCounted
{
public:
  bool Load(File*);
  SceneNode* GetSceneNode();
  const std::string& GetName() const;
  const Vec3f& GetCameraEyePos() const;
  const Vec3f& GetCameraTargetPos() const;
  void DrawGui();

  void ActivateGui();
  void DeactivateGui();

  void UpdateGui();

protected:
  std::string m_name;

  // 3D scene node
  PSceneNode m_node;

  Vec3f m_camEye;
  Vec3f m_camTarget;

  std::string m_screenGuiFilename;
  std::string m_notebookGuiFilename;

  PGuiElement m_gui;
};
}
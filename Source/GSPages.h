// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Colour.h>
#include "GSBase3d.h"
#include "MusicEvent.h"
#include "Page.h"
#include "QuestionProgress.h"

namespace Amju
{
struct Colour;
  
// * GSPages *
// Show 'pages' of questions for the user to answer.
// One page is displayed at a time. The page type depends on the type
//  of question being asked.
class GSPages : public GSBase3d
{
public:
  GSPages();
  virtual void OnActive() override;
  virtual void OnDeactive() override;
  virtual void Draw2d() override;
  virtual void Update() override;
  virtual bool OnKeyEvent(const KeyEvent& ke) override;

  void OnMusicKbEvent(const MusicKbEvent&);

  // Load list of pages from file?
  bool Load(const std::string& filename);

  void SetPage(Page* p);

  void OnCorrect();
  void OnIncorrect();
  void NextPage();
  void OnHint();
  void OnPause();
  void OnQuitConfirmCancel();

  // Start showing pages for the given topic (topic number as for Course)
  void StartTopic(int topicNum);

  void ShowGuiElement(const std::string& elemName, bool showNotHide = true);

  QuestionProgress& GetProgress();
   
protected:
  void ShowHints();
  void SetButtonEnabled(const std::string& buttonName, bool enabled);
  virtual void ReloadGui() override;

  // Load 3D and add 'teacher' avatar
  virtual void Reload3d() override;

  void UpdateHud();

protected:
  RCPtr<Page> m_page;

  // For this test attempt, keep track of questions asked, so we don't
  //  ask the same question repeatedly. 
  RCPtr<QuestionProgress> m_progress;

  // Number of pages shown in this topic session
  int m_numPagesShown = 0;

  int m_numCorrectThisSession = 0;
  int m_numIncorrectThisSession = 0;
  int m_maxNumPagesThisSession = 10;
  int m_scoreThisSession = 0;
  int m_livesThisSession = 0;
  int m_scoreMultiplier = 1;

  // User config file, for things like number of hints available/used etc.
  // Weak, owned by UserProfile
  ConfigFile* m_userConfig = nullptr;
};

typedef Singleton<GSPages> TheGSPages;
}

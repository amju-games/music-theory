// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Colour.h>
#include "GSBase3d.h"
#include "MusicEvent.h"
#include "Page.h"

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

  void OnCorrect(const Vec2f& choicePos);
  void OnIncorrect(const Vec2f& choicePos);
  void NextPage();
  void OnHint();
  void OnPause();
  void OnQuitConfirmCancel();

  // Start showing pages for the given topic (topic number as for Course)
  void StartTopic(int topicNum);

protected:
  void ShowHints();
  void SetPie(int n, const Colour& c);
  void SetButtonEnabled(const std::string& buttonName, bool enabled);
  virtual void ReloadGui() override;
  void HideTickAndCross();

  // Load 3D and add 'teacher' avatar
  virtual void Reload3d() override;

protected:
  RCPtr<Page> m_page;

  // Number of pages shown in this topic session
  int m_numPagesShown = 0;

  int m_numCorrectThisSession = 0;
  int m_numIncorrectThisSession = 0;
  int m_maxNumPagesThisSession = 10;

  // User config file, for things like number of hints available/used etc.
  // Weak, owned by UserProfile
  ConfigFile* m_userConfig = nullptr;
};

typedef Singleton<GSPages> TheGSPages;
}

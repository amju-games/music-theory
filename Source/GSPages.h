// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Colour.h>
#include "GSBase.h"
#include "MusicEvent.h"
#include "Page.h"

namespace Amju
{
struct Colour;
  
class GSPages : public GSBase
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

  void AddPage(Page* p);

  void OnCorrect();
  void OnIncorrect();
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

protected:
  //std::vector<RCPtr<Page>> m_pages;
  //int m_currentPage = 0;
  
  RCPtr<Page> m_page;

  // Number of pages shown in this topic session
  int m_numPagesShown = 0;

  int m_numCorrectThisSession = 0;
  int m_numIncorrectThisSession = 0;
  int m_maxNumPagesThisSession = 10;

  // User config file, for things like number of hints available/used etc.
  // Weak, owned by UserProfile
  ConfigFile* m_userConfig = nullptr;

  int m_numHintsAvailable = 0;
};

typedef Singleton<GSPages> TheGSPages;
}

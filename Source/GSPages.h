// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"
#include "Page.h"

namespace Amju
{
class GSPages : public GSBase
{
public:
  GSPages();
  virtual void OnActive() override;
  virtual void OnDeactive() override;
  virtual void Draw2d() override;
  virtual void Update() override;

  // Load list of pages from file?
  bool Load(const std::string& filename);

  void AddPage(Page* p);

  void OnCorrect();
  void OnIncorrect();
  void NextPage();
  void OnHint();

  // Start showing pages for the given topic (topic number as for Course)
  void StartTopic(int topicNum);

protected:
  std::vector<RCPtr<Page>> m_pages;
  int m_currentPage = 0;

  // Number of pages shown in this topic session
  int m_numPagesShown = 0;

  int m_numCorrectThisSession = 0;
  int m_numIncorrectThisSession = 0;
  int m_maxNumPagesThisSession = 10;
};

typedef Singleton<GSPages> TheGSPages;

}

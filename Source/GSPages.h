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
  virtual void Draw2d() override;
  virtual void Update() override;

  // Load list of pages from file?
  bool Load(const std::string& filename);

  void AddPage(Page* p);

  void OnCorrect();
  void OnIncorrect();

protected:
  std::vector<RCPtr<Page>> m_pages;
  int m_currentPage = 0;
};

typedef Singleton<GSPages> TheGSPages;

}

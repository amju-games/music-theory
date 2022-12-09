// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include "GSPages.h"
#include "PageMusicalTerm.h"

namespace Amju
{
GSPages::GSPages()
{
  m_guiFilename = "Gui/gs_pages.txt";
}

void GSPages::OnActive()
{
  GSBase::OnActive();
  AddPage(new PageMusicalTerm);
  m_currentPage = 0;

  // Activate new page
  m_pages[m_currentPage]->OnActive();
}

void GSPages::Draw2d()
{
  GSBase::Draw2d();

  AmjuGL::PushMatrix();
  AmjuGL::Translate(0, -0.2f, 0);
  m_pages[m_currentPage]->Draw(); 
  AmjuGL::PopMatrix();
}

void GSPages::Update()
{
  GSBase::Update();
  m_pages[m_currentPage]->Update();
}

// Load list of pages from file?
bool GSPages::Load(const std::string& filename)
{
  return true;
}

void GSPages::AddPage(Page* p)
{
  m_pages.push_back(p);
}
}


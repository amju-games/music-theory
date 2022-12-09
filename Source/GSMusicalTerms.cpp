// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSMusicalTerms.h"

namespace Amju
{
GSMusicalTerms::GSMusicalTerms()
{
  m_guiFilename = "Gui/gs_musical_terms.txt";
}

void GSMusicalTerms::OnActive()
{
  // Load dictionary of terms
  m_dictionary = new MusicalTermsDictionary;
  if (!m_dictionary->Load("musical_terms_test.txt"))
  {
    std::cout << "Failed to load musical terms.\n";
    Assert(false);
  }

  m_question.SetDictionary(m_dictionary);
}

}


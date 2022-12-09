// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "PageFactory.h"
#include "PageMultiChoice.h"
#include "PagePlayNotes.h"

namespace Amju
{
template <class T>
Page* CreateT() { return new T; }

Page* CreatePageSymbolSymbol()
{
  PageMultiChoice* page = new PageMultiChoice;
  page->SetGuiName("musical_symbol"); // page_*.txt
  page->SetDictionaryName("Course/note_lengths.txt.dictionary");
  page->SetQuestionType(QuestionType::QTYPE_SCORE);
  page->SetAnswerType(AnswerType::ATYPE_SCORE);
  return page;
}

Page* CreatePageSymbolText()
{
  PageMultiChoice* page = new PageMultiChoice;
  page->SetGuiName("musical_symbol"); // page_*.txt
  page->SetDictionaryName("Course/musical_symbols.txt.dictionary");
  page->SetQuestionType(QuestionType::QTYPE_SCORE);
  page->SetAnswerType(AnswerType::ATYPE_TEXT);
  return page;
}

Page* CreatePageTextText()
{
  PageMultiChoice* page = new PageMultiChoice;
  page->SetGuiName("musical_term"); // page_*.txt
  page->SetDictionaryName("Course/musical_terms.txt.dictionary");
  page->SetQuestionType(QuestionType::QTYPE_TEXT);
  page->SetAnswerType(AnswerType::ATYPE_TEXT);
  return page;
}

PageFactory::PageFactory()
{
//  Add(PageIntervalsMultiChoice::NAME, &CreateT<PageIntervalsMultiChoice>);
//Add(PagePlayNotes::NAME, &CreateT<PagePlayNotes>);

//  Add("page-text-symbol", CreatePageTextSymbol);
  Add("page-text-text", CreatePageTextText);
  Add("page-symbol-symbol", CreatePageSymbolSymbol);
  Add("page-symbol-text", CreatePageSymbolText);
}
}


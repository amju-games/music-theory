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
  page->SetGuiName("symbol_symbol_4"); // page_*.txt
  page->SetQuestionType(QuestionType::QTYPE_SCORE);
  page->SetAnswerType(AnswerType::ATYPE_SCORE);
  return page;
}

Page* CreatePageSymbolText()
{
  PageMultiChoice* page = new PageMultiChoice;
  page->SetGuiName("symbol_text_4"); // page_*.txt
  page->SetQuestionType(QuestionType::QTYPE_SCORE);
  page->SetAnswerType(AnswerType::ATYPE_TEXT);
  return page;
}

Page* CreatePageTextText()
{
  PageMultiChoice* page = new PageMultiChoice;
  page->SetGuiName("text_text_4"); // page_*.txt
  page->SetQuestionType(QuestionType::QTYPE_TEXT);
  page->SetAnswerType(AnswerType::ATYPE_TEXT);
  return page;
}

Page* CreatePageTextSymbol()
{
  PageMultiChoice* page = new PageMultiChoice;
  page->SetGuiName("text_symbol_4"); // page_*.txt
  page->SetQuestionType(QuestionType::QTYPE_TEXT);
  page->SetAnswerType(AnswerType::ATYPE_SCORE);
  return page;
}

PageFactory::PageFactory()
{
//  Add(PageIntervalsMultiChoice::NAME, &CreateT<PageIntervalsMultiChoice>);
//Add(PagePlayNotes::NAME, &CreateT<PagePlayNotes>);

  Add("page-text-symbol", CreatePageTextSymbol);
  Add("page-text-text", CreatePageTextText);
  Add("page-symbol-symbol", CreatePageSymbolSymbol);
  Add("page-symbol-text", CreatePageSymbolText);
}
}


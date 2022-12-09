// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Dictionary.h"
#include "GuiMusicScore.h"
#include "MusicalTermQuestion.h"
#include "PageScoreMCQBase.h"
#include "ScoreBuilder.h"

namespace Amju
{
void PageScoreMCQBase::OnActive()
{
  MusicalTermQuestion* q = new MusicalTermQuestion;
  m_question = q;

  // Get musical terms dictionary (TODO select grade)
  // This is a Resource.
  Dictionary* dic = dynamic_cast<Dictionary*>(
    TheResourceManager::Instance()->GetRes(m_dictionaryFilename));

  q->SetDictionary(dic);
  q->MakeQuestion();
  m_answers = q->GetMultiChoiceAnswers();

  // Seen this question before? If not we will give the correct answer.
  bool seen = q->QuestionSeenBefore(m_config);
  // Set flag for next time
  q->SetQuestionSeenBefore(m_config);

  m_canRemoveForHint.clear();

  // Load gui, set button callbacks
  PageMultiChoice::OnActive();

  // Set musical score display from question text
  GuiMusicScore* ms = dynamic_cast<GuiMusicScore*>(GetElementByName(m_gui, "music-score"));
  Assert(ms);
  // Simple: multiple glyphs. TODO smart ScoreBuilder.
  // Multiple glyphs split by ;
  Strings strs = Split(q->GetQuestionString(), ';');
  for (const std::string& s : strs)
  {
    ms->AddGlyphFromString(s);
  }

  //ScoreBuilder sb;
  //std::string error;
  //bool ok = sb.SetFromString(q->GetQuestionString(), &error);
  //if (!ok)
  //{
  //  std::cout << "Error in score string: " << error << "\n";
  //  Assert(0);
  //}
  //sb.Write(*ms);
}


}

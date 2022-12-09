// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Colour.h>
#include "ComposerList.h"
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

  void OnQuitButton();
  void OnQuitConfirmCancel();
  void OnQuitConfirmOK();

  // Start showing pages for the given topic (topic number as for Course)
  void StartTopic(int topicNum);

  void ShowGuiElement(const std::string& elemName, bool showNotHide = true);

  // Get progress for the currently active page. 
  // (Progress keeps track of used/unused questions)
  QuestionProgress& GetProgress();

  // Show number of hints for each hint type   
  void ShowHints();

  // Get a string to tell the player that he or she gave an incorrect answer.
  const std::string& GetIncorrectStr() const;

protected:
  void SetButtonEnabled(const std::string& buttonName, bool enabled);
  virtual void ReloadGui() override;

  // Load 3D and add 'teacher' avatar
  virtual void Reload3d() override;

  void UpdateHud();

  // Increment num pages until we find a page with unused questions. If all the 
  //  pages have all questions used, return false. 
  bool FindPageWithUnusedQuestions();

  void SetFinalScore();

protected:
  RCPtr<Page> m_page;

  // For this test attempt, keep track of questions asked, so we don't
  //  ask the same question repeatedly.
  // We keep track of questions asked FOR EACH PAGE.
  std::vector<RCPtr<QuestionProgress>> m_progress;

  // Number of pages shown in this topic session
  int m_numPagesShown = 0;

  // Current page to show player - this cycles through all pages for the topic,
  //  skipping pages where all questions are used.
  int m_currentPage = 0;

  int m_numCorrectThisSession = 0;
  int m_numIncorrectThisSession = 0;
  int m_maxNumPagesThisSession = 10;
  int m_scoreThisSession = 0;
  int m_livesThisSession = 0;
  int m_scoreMultiplier = 1;
  int m_maxMark = 0; // maximum mark for the current topic

  // User config file, for things like number of hints available/used etc.
  // Weak, owned by UserProfile
  ConfigFile* m_userConfig = nullptr;

  // Each topic/classroom has a Composer, who asks the questions and
  //  explains answers, etc.
  RCPtr<const Composer> m_composer;
  // Count consecutive correct or incorrect answers
  int m_correctStreak = 0;
  int m_incorrectStreak = 0;
};

typedef Singleton<GSPages> TheGSPages;
}

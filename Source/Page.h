// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <GuiButton.h>
#include <RCPtr.h>
#include <StringUtils.h>
#include "PageQuestion.h"
#include "Question.h"

namespace Amju
{
class ConfigFile;
class Dictionary;
class GameState;
struct MusicKbEvent;

// Call to load a new page, assuming the current game state is GSPages.
void GoToNextPage();

// * Page *
// A page is one screenful of information, usually in the form of a "test" or question. 
class Page : public RefCounted
{
public:
  virtual void OnActive();
  virtual void OnDeactive();
  virtual void Draw();
  virtual void Update();

  // Load any extra info from a vector of strings.
  // Default impl expects strs[1] to be path to a dictionary
  virtual bool Load(const Strings& strs);

  // Pages present info stored in "dictionaries" - text files grouping pairs/tuples of strings.
  void SetDictionaryName(const std::string& dictionaryName);

  Dictionary* GetDictionary();

  // Allows us to set different GUIs for the same Page type, as many pages could be
  //  essentially the same, just presented differently.
  void SetGuiName(const std::string& guiName);

  GuiElement* GetGui();

  // Get non-graphical question, i.e. what we ask the user to do
  Question* GetQuestion();

  // Enable/disable buttons, for modal dialog like quit confirm 
  virtual void SetIsEnabled(bool enabled);

  // Set config file to save any state we want to remember about this page/topic
  void SetConfigFile(ConfigFile* cf);

  void SetGameState(GameState* gs);

  // Called when user taps Hint button -- show page-specific hint.
  virtual void OnHint() {}
  virtual bool CanGetHint() { return true; }
  virtual void ShowCorrectAnswer() {}

  // Called when user taps piano keyboard
  virtual void OnMusicKbEvent(const MusicKbEvent&) {}

  // Sigh, there is a difference in behaviour if we are showing something new, so
  //  set this flag if user is learning a new thing.
  void SetIsLearnMode(bool learn);
  bool IsLearnMode() const;

  // Set the page question, which displays questions using UI
  void SetPageQuestion(PageQuestion*);

protected:
  GuiButton* GetHintButton() const; // convenience function
  
  void SetQuestion(Question*);

  // Use the page question to show the question to the user
  void SetUpQuestionUI();

  // Send timed Message to go to next page
  void SendNextPageMessage();

protected:
  // Base gui file name - we append current orientation and reload if the orientation changes.
  std::string m_guiName;

  // Dictionary to load: question selects an entry from this.
  std::string m_dictionaryFilename;

  // Layout of question, etc.
  PGuiElement m_gui;

  // Non-graphical representation of the question presented
  RCPtr<Question> m_question;

  // Visible UI representation of question
  RCPtr<PageQuestion> m_pageQuestion;

  // The game state which owns this page 
  // TODO This will always be GSPages, no?
  GameState* m_gs = nullptr;

  bool m_isLearnMode = false;

  // Config file to save any state we want to remember about this page/topic
  RCPtr<ConfigFile> m_config;
};
}

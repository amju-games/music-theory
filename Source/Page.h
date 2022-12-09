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
struct MusicKbEvent;
class GSPages;

// Call to load a new page, assuming the current game state is GSPages.
void GoToNextPage();

// Convenience function: get the GSPages game state, but only if it's
//  currently active.
GSPages* GetPagesState();

// * Page *
// A page is one screenful of information, usually in the form of a "test" or question. 
class Page : public RefCounted
{
public:
  // Send timed Message to go to next page
  static void SendNextPageMessage();

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

  const std::string& GetInstructionText() const { return m_instructionText; }

  // Set config file to save any state we want to remember about this page/topic
  void SetConfigFile(ConfigFile* cf);

  // Called when user taps Hint button -- show page-specific hint.
  virtual void OnHint() {}
  virtual bool CanGetHint() { return true; }
  virtual void ShowCorrectAnswer() {}

  // Called when user taps piano keyboard
  virtual void OnMusicKbEvent(const MusicKbEvent&) {}

  // Set the page question, which displays questions using UI
  void SetPageQuestion(PageQuestion*);

protected:
  GuiButton* GetHintButton() const; // convenience function
  
  void SetQuestion(Question*);

  // Use the page question to show the question to the user
  void SetUpQuestionUI();

protected:
  // Base gui file name - we append current orientation 
  //  and reload if the orientation changes.
  std::string m_guiName;

  // Dictionary to load: question selects an entry from this.
  std::string m_dictionaryFilename;

  // Layout of question, etc.
  PGuiElement m_gui;

  // Non-graphical representation of the question presented
  RCPtr<Question> m_question;

  // Visible UI representation of question
  RCPtr<PageQuestion> m_pageQuestion;

  // Config file to save any state we want to remember about this page/topic
  RCPtr<ConfigFile> m_config;

  // (Localised) text to display to the player to give instructions for 
  //  the page - e.g. "Tap on the correct answer!"
  std::string m_instructionText;
};
}

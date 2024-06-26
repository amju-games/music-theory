// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiButton.h>
#include <RCPtr.h>
#include <StringUtils.h>
#include "Dictionary.h"
#include "PageQuestion.h"
#include "Question.h"

namespace Amju
{
class ConfigFile;
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

  std::string GetInfo() const; // Get page name and other info, for authoring info

  virtual void OnActive();
  virtual void OnDeactive();
  virtual void Draw();
  virtual void Update();

  // Load any extra info from a vector of strings.
  // Default impl expects strs[1] to be path to a dictionary
  virtual bool Load(const Strings& strs);

  // Pages present info stored in "dictionaries" - text files grouping pairs/tuples of strings.
  void SetDictionaryName(const std::string& dictionaryName);

  // Pages always have a "dictionary" - a list of question/answer/explanation 
  //  tuples.
  // Calling this function does NOT reload the dictionary.
  Dictionary* GetDictionary();

  // Allows us to set different GUIs for the same Page type, as many pages could be
  //  essentially the same, just presented differently.
  void SetGuiName(const std::string& guiName);

  GuiElement* GetGui();

  // Get non-graphical question, i.e. what we ask the user to do
  Question* GetQuestion();

  const std::string& GetInstructionText() const { return m_instructionText; }

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

  // Load the dictionary (it's a resource in release mode, but we reload it in
  //  debug mode).
  Dictionary* LoadDictionary();

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

  // (Localised) text to display to the player to give instructions for 
  //  the page - e.g. "Tap on the correct answer!"
  std::string m_instructionText;

  RCPtr<Dictionary> m_dictionary;
};
}

// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <RCPtr.h>

namespace Amju
{
class Page;

// * PageQuestion *
// Base class for Question UI - i.e. the part which asks the user
//  to do something. E.g. displays text, or a musical symbol,
//  and the user then responds.
class PageQuestion : public RefCounted
{
public:
  virtual ~PageQuestion() = default;
  virtual void SetUp() = 0;
  void SetPage(Page* page);

  // Called when player answers question (correctly or incorrectly)
  virtual void OnPlayerChoice() {}

protected:
  // Point back to owning Page: weak pointer
  Page* m_page = nullptr;
};

class PageQuestionScore : public PageQuestion
{
public:
  virtual void SetUp() override;
  virtual void OnPlayerChoice() override;
};

class PageQuestionText : public PageQuestion
{
public:
  virtual void SetUp() override;
};
}


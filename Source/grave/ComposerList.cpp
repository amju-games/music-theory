// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <File.h>
#include <LoadVec2.h>
#include "ComposerList.h"

namespace Amju
{
bool Composer::Load(File* f)
{
  if (!f->GetDataLine(&m_id))
  {
    f->ReportError("Expected composer ID");
    return false;
  }
  
  if (!f->GetDataLine(&m_topic))
  {
    f->ReportError("Expected composer topic");
    return false;
  }

  if (!f->GetDataLine(&m_displayName))
  {
    f->ReportError("Expected composer display name");
    return false;
  }

  if (!f->GetDataLine(&m_avatarStr))
  {
    f->ReportError("Expected composer avatar string");
    return false;
  }

  std::string s;
  // Correct strings
  if (!f->GetLocalisedString(&s))
  {
    f->ReportError("Expected composer correct strings");
    return false;
  }
  m_correctStrs = Split(s, '|'); // strings split with | char, in all languages

  // Incorrect
  if (!f->GetLocalisedString(&s))
  {
    f->ReportError("Expected composer incorrect strings");
    return false;
  }
  m_incorrectStrs = Split(s, '|'); // strings split with | char, in all languages
  
  // Load year of birth and death
  if (!LoadVec2(f, &m_yearBirthDeath))
  {
    f->ReportError("Expected composer year of birth, year of death");
  }

  // Biographical info
  if (!f->GetLocalisedString(&m_bio))
  {
    f->ReportError("Expected composer bio");
    return false;
  }

  return true;
}

const std::string& Composer::GetDisplayName() const
{
  return m_displayName;
}

const std::string& Composer::GetAvatarStr() const
{
  return m_avatarStr;
}

const std::string& Composer::GetIncorrectStr(int streak) const
{
  int s = m_incorrectStrs.size() - 1;
  if (streak > s)
  {
    streak = s;
  }
  return m_incorrectStrs[streak];
}

const std::string& Composer::GetCorrectStr(int streak) const
{
  int s = m_correctStrs.size() - 1;
  if (streak > s)
  {
    streak = s;
  }
  return m_correctStrs[streak];
}

const Composer* ComposerList::GetComposerById(const std::string& composerId) const
{
  for (const Composer* c : m_composers)
  {
    if (c->m_id == composerId)
    {
      return c;
    }
  }
  Assert(0);
  return nullptr;
}

const Composer* ComposerList::GetComposerForTopic(const std::string& composerId) const
{
  return m_composers[0]; // TODO TEMP TEST

  for (const Composer* c : m_composers)
  {
    if (c->m_topic == composerId)
    {
      return c;
    }
  }
  Assert(0);
  return nullptr;
}

bool ComposerList::Load(const std::string& filename)
{
  m_composers.clear();
  File f;
  if (!f.OpenRead(filename))
  {
    return false;
  }
  int num = 0;
  if (!f.GetInteger(&num))
  {
    f.ReportError("Expected number of composers");
    return false;
  }
  for (int i = 0; i < num; i++)
  {
    RCPtr<Composer> c = new Composer;
    if (!c->Load(&f))
    {
      f.ReportError("Failed to load composer");
      Assert(0);
      return false;
    }
    m_composers.push_back(c);
  }
  return true;
}

ComposerList& GetComposerList()
{
  static ComposerList complist;
  return complist;
}
}

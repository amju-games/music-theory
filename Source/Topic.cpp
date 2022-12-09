// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <File.h>
#include "PageFactory.h"
#include "Topic.h"

namespace Amju
{
Topic::Topic(const std::string& id)
{
  SetId(id);
}

const std::string& Topic::GetId() const
{
  return m_id;
}

void Topic::SetId(const std::string& id)
{
  m_id = id;
}

const std::string& Topic::GetDisplayName() const
{
  return m_displayName;
}

void Topic::SetDisplayName(const std::string& displayName)
{
  m_displayName = displayName;
}

void Topic::AddTopicThisUnlocks(const std::string& unlocks)
{
}

void Topic::OnCompleted()
{
}

bool Topic::Load(File* f)
{
  if (!f->GetDataLine(&m_id))
  {
    f->ReportError("Expected topic ID");
    return false;
  }

  if (!f->GetDataLine(&m_displayName))
  {
    f->ReportError("Expected topic display name");
    return false;
  }

  // Read pages
  int numPages = 0;
  if (!f->GetInteger(&numPages))
  {
    f->ReportError("Expected number of pages");
    return false;
  }

  std::string line;
  if (!f->GetDataLine(&line))
  {
    f->ReportError("Expected page type name");
    return false;
  }

  for (int i = 0; i < numPages; i++)
  {
    // TODO Options, number of repeats, etc.
    RCPtr<Page> page = ThePageFactory::Instance()->Create(line);
    if (!page)
    {
      f->ReportError("Unexpected page name");
      return false;
    }
    m_pages.push_back(page);
  }
  return true;
}

int Topic::GetNumPages() const
{
  return m_pages.size();
}

Page* Topic::GetPage(int n)
{
  return m_pages[n];
}

}


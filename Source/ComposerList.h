// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#pragma once

#include <string>
#include <vector>
#include <RCPtr.h>
#include <StringUtils.h>
#include <Vec2.h>

namespace Amju
{
class File;

// * Composer *
// Info about a Great Composer of the past, e.g. J.S. Bach etc. Has some
//  biographical info, strings for when the player gets answers correct
//  and incorrect, and a string to set a GuiAvatar.
class Composer : public RefCounted
{
public:
  bool Load(File*);

  // Get composer-specific string for an incorrect answer, with the streak
  //  number used to make the sequence make sense, not just disconnected.
  const std::string& GetIncorrectStr(int streak) const;

  // Get composer-specific string for a correct answer, with the streak
  //  number used to make the sequence make sense, not just disconnected.
  const std::string& GetCorrectStr(int streak) const;

private:
  friend class ComposerList;

  std::string m_id;
  std::string m_topic; // topic unique ID which this Composer haunts
  std::string m_displayName;
  std::string m_avatarStr;
  Strings m_correctStrs;
  Strings m_incorrectStrs;
  Vec2i m_yearBirthDeath;
  std::string m_bio;
};

// * ComposerList *
// Loads and stores a sequence of Composers.
class ComposerList
{
public:
  // Get composer given his or her unique ID
  const Composer* GetComposerById(const std::string& composerId) const;

  // Get the composer who haunts the given topic/classroom
  const Composer* GetComposerForTopic(const std::string& composerId) const;

  bool Load(const std::string& filename);

private:
  // Vector - we just do a linear search in GetComposer
  std::vector<RCPtr<Composer>> m_composers;
};

// Not A Singleton Guv, but use this to get the instance we will always 
//  want to use, except for Testing.
ComposerList& GetComposerList();
}

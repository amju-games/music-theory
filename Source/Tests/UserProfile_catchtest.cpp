// * Amjula music theory *
// (c) Copyright 2017 Juliet Colman

#include "catch.hpp"
#include "HeroGameRound.h"
#include "UserProfile.h"

using namespace Amju;

class MyConfigFile : public ConfigFile
{
public:
  void Dump()
  {
    for (const auto& [k, v] : m_values)
    {
      std::cout << k << ": " << v << "\n";
    }
    std::cout << m_values.size() << " pairs.\n";
  }
};

// Convenience create function
static HeroGameRound MakeRound(const std::string& songName, int level)
{
  HeroGameRound r;
  r.m_name = songName;
  r.m_level = level;
  return r;
}

TEST_CASE("Unlocked songs", "[UserProfile]")
{
  // Test unlocked songs logic:
  // All songs in level <n> are locked unless the songs in level <n-1>
  //  are completed. Songs in level 0 are always unlocked.
  // User profile stores which songs have been completed.
  // GameRoundManager stores which level each song belongs to.

  const int hiScore = 1000; // doesn't matter
  const bool like = true; // doesn't matter
  const bool COMPLETED = true;
  const bool NOT_COMPLETED = false; // not sure we need this? Yes, we do.

  const std::string SONG1 = "song1";
  const std::string SONG2 = "song2";
  const std::string SONG3 = "song3";
  const std::string SONG4 = "song4";

  SECTION("Songs in Level 0 are always unlocked.")
  {
    GameRoundManager grm;
    // Set the level for each song ('game round' - TODO rename)
    grm.AddGameRound(MakeRound(SONG1, 0)); // song1 is in level 0
    grm.AddGameRound(MakeRound(SONG4, 0)); // song4 is in level 0

    // Set up song 1 is completed
    MockUserProfile user;
    user.SetSongPlayerInfo(SongPlayerInfo{SONG1, hiScore, COMPLETED, like});

    auto unlocked = CalcUnlockedSongNames(&grm, &user);

    REQUIRE(unlocked.contains(SONG1)); // it's in level 0, so unlocked
    REQUIRE(!unlocked.contains(SONG2)); // song not in game rounds
    REQUIRE(unlocked.contains(SONG4)); // it's in level 0, doesn't
      // matter that it's not in player info
  }

  SECTION("Song in level 1 is not unlocked, prev level (1 song) not completed.")
  {
    GameRoundManager grm;
    grm.AddGameRound(MakeRound(SONG1, 0)); // song1 is in level 0
    grm.AddGameRound(MakeRound(SONG2, 1)); // song2 is in level 1

    MockUserProfile user;
    user.SetSongPlayerInfo(SongPlayerInfo{SONG1, hiScore, NOT_COMPLETED, like});

    auto unlocked = CalcUnlockedSongNames(&grm, &user);

    // NOT unlocked because song1 is not completed 
    REQUIRE(!unlocked.contains(SONG2)); 
  }

  SECTION("Song in level 1 is not unlocked, prev level (2 songs) not completed.")
  {
    GameRoundManager grm;
    grm.AddGameRound(MakeRound(SONG1, 0)); // song1 is in level 0
    grm.AddGameRound(MakeRound(SONG2, 0)); // song2 is in level 0
    grm.AddGameRound(MakeRound(SONG3, 1)); // song3 is in level 1

    MockUserProfile user;
    user.SetSongPlayerInfo(SongPlayerInfo{SONG1, hiScore, NOT_COMPLETED, like});
    user.SetSongPlayerInfo(SongPlayerInfo{SONG2, hiScore, COMPLETED, like});

    auto unlocked = CalcUnlockedSongNames(&grm, &user);

    // NOT unlocked because song1 is not completed 
    REQUIRE(!unlocked.contains(SONG3)); 
  }

  SECTION("Song in level 1 is unlocked, prev level (1 song) is completed.")
  {
    GameRoundManager grm;
    grm.AddGameRound(MakeRound(SONG1, 0)); // song1 is in level 0
    grm.AddGameRound(MakeRound(SONG2, 1)); // song2 is in level 1

    MockUserProfile user;
    user.SetSongPlayerInfo(SongPlayerInfo{SONG1, hiScore, COMPLETED, like});

    auto unlocked = CalcUnlockedSongNames(&grm, &user);

    // Song2 IS unlocked because song1 is completed 
    REQUIRE(unlocked.contains(SONG2)); 
  }

  SECTION("Songs in level 1 are unlocked, prev level (2 songs) is completed.")
  {
    GameRoundManager grm;
    grm.AddGameRound(MakeRound(SONG1, 0)); // song1 is in level 0
    grm.AddGameRound(MakeRound(SONG2, 0)); // song2 is in level 0
    grm.AddGameRound(MakeRound(SONG3, 1)); // song3 is in level 1
    grm.AddGameRound(MakeRound(SONG4, 1)); // song4 is in level 1

    MockUserProfile user;
    user.SetSongPlayerInfo(SongPlayerInfo{SONG1, hiScore, COMPLETED, like});
    user.SetSongPlayerInfo(SongPlayerInfo{SONG2, hiScore, COMPLETED, like});

    auto unlocked = CalcUnlockedSongNames(&grm, &user);

    // All songs in level 1 unlocked because all songs in level 0 completed
    REQUIRE(unlocked.contains(SONG3)); 
    REQUIRE(unlocked.contains(SONG4)); 
  }
}


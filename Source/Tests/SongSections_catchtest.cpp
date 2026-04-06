#include "catch.hpp"
#include "FindSongSections.h"

using namespace Amju;

TEST_CASE("Insert new sections", "[SongSections]")
{
  Sections sections 
  {
    Section(10, 30),
    Section(32, 124),
    Section(160, 252),
  };

  std::vector<int> ids { 19, 41, 89 };

  // No note events (the 3rd param) so we don't adjust for rests.
  // That's something that should be tested!! TODO!!
  InsertNewSections(sections, ids, {});

for (const auto& s : sections)
{
  std::cout << s << "\n";
}

  REQUIRE(sections[0] == Section(10, 19));
  REQUIRE(sections[1] == Section(19, 30));
  REQUIRE(sections[2] == Section(30, 32));
  REQUIRE(sections[3] == Section(32, 41));
  REQUIRE(sections[4] == Section(41, 89));
  REQUIRE(sections[5] == Section(89, 124));
  REQUIRE(sections[6] == Section(124, 160));
  REQUIRE(sections[7] == Section(160, 252));
}



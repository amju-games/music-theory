#include "catch.hpp"
#include "FindSongSections.h"

using namespace Amju;

TEST_CASE("Insert new sections", "[SongSections]")
{
  Sections sections 
  {
    Section(10, 30),
    Section(32, 124),
    Section(138, 158),
    Section(160, 252),
  };

  std::vector<int> ids { 19, 41, 49, 73, 89 };

  InsertNewSections(sections, ids);

for (const auto& s : sections)
{
  std::cout << s << "\n";
}

  REQUIRE(sections[0] == Section(10, 19));
  REQUIRE(sections[1] == Section(19, 30));
  REQUIRE(sections[2] == Section(32, 41));
  REQUIRE(sections[3] == Section(41, 49));
  REQUIRE(sections[4] == Section(49, 73));
  REQUIRE(sections[5] == Section(73, 89));
  REQUIRE(sections[6] == Section(89, 124));
  REQUIRE(sections[7] == Section(138, 158));
  REQUIRE(sections[8] == Section(160, 252));
}



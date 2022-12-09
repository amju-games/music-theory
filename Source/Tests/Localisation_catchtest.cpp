// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "catch.hpp"
#include <StringUtils.h>

using namespace Amju;

TEST_CASE("Replace escaped characters", "Localisation")
{
  std::string res = ReplaceUtf8EscapedChars("a\\x80b");
  REQUIRE(res.size() == 3);
  REQUIRE(res[0] == 'a');
  REQUIRE(res[1] == (char)0x80);
  REQUIRE(res[2] == 'b');
}

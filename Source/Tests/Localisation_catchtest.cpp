// * Amjula music theory *
// (c) Copyright 2017 Juliet Colman

#include "catch.hpp"
#include <Localise.h>
#include <StringUtils.h>

using namespace Amju;

namespace
{
std::string GetString(int i)
{
  return std::to_string(i);
}
} // anon namespace

TEST_CASE("LookupMulti", "Localisation")
{
  REQUIRE(LookupMulti("$$$1", GetString) == "1"); 
  REQUIRE(LookupMulti("Hello $$$1", GetString) == "Hello 1"); 
  REQUIRE(LookupMulti("$$$1 $$$2", GetString) == "1 2"); 
  REQUIRE(LookupMulti("$$$1$$$2", GetString) == "12"); 
}

TEST_CASE("Replace escaped characters", "Localisation")
{
  std::string res = ReplaceUtf8EscapedChars("a\\x80b");
  REQUIRE(res.size() == 3);
  REQUIRE(res[0] == 'a');
  REQUIRE(res[1] == (char)0x80);
  REQUIRE(res[2] == 'b');
}

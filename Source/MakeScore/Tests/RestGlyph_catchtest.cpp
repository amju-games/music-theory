#include "catch.hpp"
#include "RestGlyph.h"

TEST_CASE("Recognise rest tokens", "[RestGlyph]")
{
  REQUIRE(IsImmediateRest("r"));
  REQUIRE(IsImmediateRest("R"));
  REQUIRE(IsDeferredRest("<r>"));
  // No such token as deferred whole bar rest
  REQUIRE_FALSE(IsDeferredRest("<R>"));

  // For a crotchet rest, input is `<c> r`, etc.
  REQUIRE_FALSE(IsImmediateRest("cr"));

  REQUIRE(IsWholeBarRest("R"));
  REQUIRE_FALSE(IsWholeBarRest("r"));
}

TEST_CASE("Whole bar rests", "[RestGlyph]")
{
  // The first ctor param is the duration, NOT the rest token!

  RestGlyph rest("c", false, 0);
  REQUIRE_FALSE(rest.IsWholeBarRest());

  RestGlyph rest2("c", true, 0);
  REQUIRE(rest2.IsWholeBarRest());
}



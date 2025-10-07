// * Amjula music theory *
// (c) Copyright 2017 Juliet Colman

#include "catch.hpp"
#include "Dictionary.h"

using namespace Amju;

// Test Dictionary class

TEST_CASE("Add localised strs to Dictionary", "Dictionary")
{
  // TODO Fix up Localise so it can be tested!
//  Localise loc;
}

TEST_CASE("Add raw strs to Dictionary", "Dictionary")
{
  Dictionary dic;

  dic.AddTerm({"abc", "def", "ghi"});
  dic.AddTerm({"jkl", "mno", "pqr"});

  REQUIRE(dic.GetNumTerms() == 2);

  std::string s1, s2, s3;

  bool foundTerm = dic.GetTerm(0, &s1, &s2);
  REQUIRE(foundTerm);
  REQUIRE(s1 == "abc");
  REQUIRE(s2 == "def");

  foundTerm = dic.GetTerm(0, &s1, &s2, nullptr);
  REQUIRE(foundTerm);
  REQUIRE(s1 == "abc");
  REQUIRE(s2 == "def");

  foundTerm = dic.GetTerm(0, &s1, &s2, &s3);
  REQUIRE(foundTerm);
  REQUIRE(s1 == "abc");
  REQUIRE(s2 == "def");
  REQUIRE(s3 == "ghi");
}



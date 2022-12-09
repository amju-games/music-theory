// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "catch.hpp"
#include "Question.h"

using namespace Amju;

TEST_CASE("Terms can be added and retrieved", "[MusicalTermsDictionary]") 
{
	MusicalTermsDictionary dic;

	SECTION("Adding terms")
	{
		dic.AddTerm("Soft", "Piano");
		dic.AddTerm("Loud", "Forte");
		dic.AddTerm("Sweetly", "Dolce");
		dic.AddTerm("Gracefully", "Grazioso");

		REQUIRE(dic.GetNumTerms() == 4);

		SECTION("Retrieving terms")
		{
			std::string english, foreign;
			dic.GetTerm(0, &english, &foreign);
			REQUIRE(english == "Soft");

		}
	}
}


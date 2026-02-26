#include <iostream>
#include "catch.hpp"
#include "TimeValue.h"

TEST_CASE("Set Times from token", "[Times]")
{
  REQUIRE(Times("qqq").GetTimeType() == TimeType::QQQ);
  REQUIRE(Times("qqq.").GetTimeType() == TimeType::DOTTED_QQQ);
  REQUIRE(Times("qq").GetTimeType() == TimeType::SEMIQUAVER);
  REQUIRE(Times("qq.").GetTimeType() == TimeType::DOTTED_SEMIQUAVER);
  REQUIRE(Times("c").GetTimeType() == TimeType::CROTCHET);
  REQUIRE(Times("c.").GetTimeType() == TimeType::DOTTED_CROTCHET);
  REQUIRE(Times("m").GetTimeType() == TimeType::MINIM);
  REQUIRE(Times("m.").GetTimeType() == TimeType::DOTTED_MINIM);

  REQUIRE(Times("e").GetTimeType() == TimeType::ERROR);
  REQUIRE(Times("r").GetTimeType() == TimeType::ERROR);
  REQUIRE(Times(".").GetTimeType() == TimeType::ERROR);
}


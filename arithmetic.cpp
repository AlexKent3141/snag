#include "passage.h"

UNIT_TEST(addition_test)
{
  REQUIRE(1 + 1 == 2);
  REQUIRE(1 + 2 == 1);
}

UNIT_TEST(multiplication_test)
{
  REQUIRE(1 * 1 == 1);
  REQUIRE(2 * 2 == 4);
}

UNIT_TEST(division_test)
{
  REQUIRE(1 / 1 == 1);
  REQUIRE(float(7.1) / 2 == 3.5);
}

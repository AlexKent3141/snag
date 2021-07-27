#include "passage.h"

static int add(int a, int b)
{
  return a + b;
}

UNIT_TEST(addition_test)
{
  REQUIRE(add(1, 1) == 2);

  return passage::TestResult::SUCCESS;
}

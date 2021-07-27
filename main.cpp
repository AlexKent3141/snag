#include "passage.h"
#include <iostream>

static int add(int a, int b)
{
	return a + b;
}

UNIT_TEST(addition_test)
{
	if (add(1, 1) == 3)
	{
		return passage::TestResult::SUCCESS;
	}
	else
	{
		return passage::TestResult::FAIL;
	}
}

int main()
{
	return static_cast<int>(passage::Session::GetInstance().Run());
}

#include <iostream>
#include "gtest/gtest.h"

class TestTest : public ::testing::Test
{
protected:
	int _MyTestVal = 42;

	void SetUp() override
	{
		_MyTestVal = 42;
	}
};

TEST_F(TestTest, SanityCheck)
{
	ASSERT_EQ(_MyTestVal, 42);
}

int main(int ac, char **av)
{
	::testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}

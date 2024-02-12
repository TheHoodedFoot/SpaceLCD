#include <stdint.h>

#include "bgr.h"
//#include "t_bgr.h"

#include "unity_fixture.h"

TEST_GROUP(BgrGroup);

TEST_SETUP(BgrGroup) {}

TEST_TEAR_DOWN(BgrGroup) {}

/***********************
 *  Write a test list  *
 ***********************/

/* What is the set of tests which, when passes, demonstrates that the code
 * works correctly? */

/* When writing a test, imagine the perfect interface for that function */

/* What function do we wish existed? */

TEST(BgrGroup, BgrTest)
{
	/* What are the requirements to test 'x'? */
	/* 	What does the object do? */
	/* 	How does it interact with the data or hardware it controls? */
	/* 	How can we make it testable? */

	uint8_t bgrdata[] = {0x1f, 0x00, 0x1f, 0x00};

	rgbtobgr(bgrdata, 4);

	TEST_ASSERT_EQUAL(0x00, bgrdata[0]);
	TEST_ASSERT_EQUAL(0xf8, bgrdata[1]);
	TEST_ASSERT_EQUAL(0x00, bgrdata[2]);
	TEST_ASSERT_EQUAL(0xf8, bgrdata[3]);

	addheader(&bgrdata[0], 0x1234, 0x11);

	TEST_ASSERT_EQUAL(0x11, bgrdata[0]);
	TEST_ASSERT_EQUAL(0x0f, bgrdata[1]);
	TEST_ASSERT_EQUAL(0x34, bgrdata[2]);
	TEST_ASSERT_EQUAL(0x12, bgrdata[3]);
}


TEST_GROUP_RUNNER(BgrGroup)
{ /* Add a line below for each unit test */

	RUN_TEST_CASE(BgrGroup, BgrTest);
}

static void
runAllTests()
{
	RUN_TEST_GROUP(BgrGroup);
}

int
main(int argc, const char *argv[])
{
	return UnityMain(argc, argv, runAllTests);
}

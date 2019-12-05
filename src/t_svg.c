#include <string.h>
#include "svg.hpp"

#include "unity_fixture.h"

TEST_GROUP(SvgGroup);

TEST_SETUP(SvgGroup) {}

TEST_TEAR_DOWN(SvgGroup) {}

TEST(SvgGroup, ParseSVGTest)
{
	const char *whiterect =
		"<svg width=\"640\" height=\"150\" version=\"1.1\"><rect "
		"x=\"1\" y=\"0\" width=\"638\" height=\"150\" ry=\"0\" "
		"fill=\"#ffffff\"/></svg>";

	unsigned char *buffer = (unsigned char *)malloc(BITMAP_BYTES);
	svgtorgb565(whiterect, strlen(whiterect), (char *)buffer);

	TEST_ASSERT_EQUAL(0x00, *(buffer));
	TEST_ASSERT_EQUAL(0xff, *(buffer + 2));
	TEST_ASSERT_EQUAL(0xff, *(buffer + 191996));
	TEST_ASSERT_EQUAL(0x00, *(buffer + 191998));

	free(buffer);
}


TEST_GROUP_RUNNER(SvgGroup)
{ /* Add a line below for each unit test */

	RUN_TEST_CASE(SvgGroup, ParseSVGTest);
}

static void
runAllTests()
{
	RUN_TEST_GROUP(SvgGroup);
}

int
main(int argc, const char *argv[])
{
	return UnityMain(argc, argv, runAllTests);
}

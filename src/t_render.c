#include <string.h>
#include "render.h"

#include "unity_fixture.h"

TEST_GROUP(RenderGroup);

TEST_SETUP(RenderGroup) {}

TEST_TEAR_DOWN(RenderGroup) {}

TEST(RenderGroup, ParseSVGTest)
{
	const char *whiterect =
		"<svg width=\"640\" height=\"150\" version=\"1.1\"><rect "
		"x=\"1\" y=\"0\" width=\"638\" height=\"150\" ry=\"0\" "
		"fill=\"#ffffff\"/></svg>";

	void *buffer = malloc(BITMAP_BYTES);
	svgtobgr565(whiterect, strlen(whiterect), buffer);

	TEST_ASSERT_EQUAL(0x00, *(buffer));
	TEST_ASSERT_EQUAL(0xff, *(buffer + 2));
	TEST_ASSERT_EQUAL(0xff, *(buffer + 191996));
	TEST_ASSERT_EQUAL(0x00, *(buffer + 191998));

	free(buffer);
}


TEST_GROUP_RUNNER(RenderGroup)
{ /* Add a line below for each unit test */

	RUN_TEST_CASE(RenderGroup, ParseSVGTest);
}

static void
runAllTests()
{
	RUN_TEST_GROUP(RenderGroup);
}

int
main(int argc, const char *argv[])
{
	return UnityMain(argc, argv, runAllTests);
}

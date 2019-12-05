#include <stdio.h>
#include "deflate.h"
#include "unity_fixture.h"

TEST_GROUP(DeflateGroup);

TEST_SETUP(DeflateGroup) {}

TEST_TEAR_DOWN(DeflateGroup) {}

TEST(DeflateGroup, compresstest)
{
	/* All we can really do to test the decompression function
	 * is to compress then compare to pre-calculated examples. */

#define BUFFER_MAX 128
#define ARRAY_COUNT(name) sizeof(name) / sizeof(name[0])

	unsigned char buffer[BUFFER_MAX];

	unsigned char zero_in[] = {0x00};
	unsigned char zero_out[] = {0x63, 0x00, 0x00};

	int result = crush(zero_in, buffer, ARRAY_COUNT(zero_in));
	TEST_ASSERT_EQUAL(ARRAY_COUNT(zero_out), result);
	TEST_ASSERT_EQUAL_MEMORY(zero_out, buffer, ARRAY_COUNT(zero_out));

	unsigned char hello_in[] = {'H', 'e', 'l', 'l', 'o'};
	unsigned char hello_out[] = {0xf3, 0x48, 0xcd, 0xc9,
				     0xc9, 0x07, 0x00};

	result = crush(hello_in, buffer, ARRAY_COUNT(hello_in));
	TEST_ASSERT_EQUAL(ARRAY_COUNT(hello_out), result);
	TEST_ASSERT_EQUAL_MEMORY(hello_out, buffer, ARRAY_COUNT(hello_out));
}

TEST_GROUP_RUNNER(DeflateGroup) { RUN_TEST_CASE(DeflateGroup, compresstest); }

static void
runAllTests()
{
	RUN_TEST_GROUP(DeflateGroup);
}

int
main(int argc, const char *argv[])
{
	return UnityMain(argc, argv, runAllTests);
}

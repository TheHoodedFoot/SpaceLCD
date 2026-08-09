/* Test: send an SVG as a PARTIAL (alpha blend) update */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libspacelcd.h"

/* Read entire file into a buffer */
static char *
read_file(const char *path)
{
	FILE *f = fopen(path, "r");
	if (!f) {
		fprintf(stderr, "Cannot open %s\n", path);
		return NULL;
	}
	fseek(f, 0, SEEK_END);
	long sz = ftell(f);
	fseek(f, 0, SEEK_SET);
	char *buf = (char *)malloc(sz + 1);
	fread(buf, 1, sz, f);
	buf[sz] = '\0';
	fclose(f);
	return buf;
}

int
main(int argc, char **argv)
{
	const char *path = "../res/img/logo.svg";
	if (argc > 1) path = argv[1];

	char *svg = read_file(path);
	if (!svg) return 1;

	printf("Sending %s as PARTIAL update...\n", path);
	printf("Expected: SVG renders on top of existing screen content.\n");
	printf("          Rest of screen should be unchanged.\n");

	int rc = svgtolcd_partial(svg, 0);
	if (rc != 0) {
		fprintf(stderr, "svgtolcd_partial returned %d\n", rc);
		free(svg);
		return 1;
	}

	printf("Done.\n");
	free(svg);
	return 0;
}

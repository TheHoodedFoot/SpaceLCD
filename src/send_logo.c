/* Send logo.svg to the LCD screen — convenience test target */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libspacelcd.h"

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
	if (!buf) {
		fclose(f);
		return NULL;
	}
	fread(buf, 1, sz, f);
	buf[sz] = '\0';
	fclose(f);
	return buf;
}

int
main(int argc, char **argv)
{
	const char *path = LOGO_PATH;
	if (argc > 1) path = argv[1];

	char *svg = read_file(path);
	if (!svg) return 1;

	printf("Sending %s to LCD...\n", path);
	int rc = svgtolcd(svg, EFFECT_CUT);
	if (rc != 0) {
		fprintf(stderr, "svgtolcd returned %d\n", rc);
		free(svg);
		return 1;
	}
	printf("Done.\n");
	free(svg);
	return 0;
}

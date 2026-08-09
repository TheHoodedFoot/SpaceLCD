/* Test: send a solid-colour bitmap as a PARTIAL (alpha blend) update */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libspacelcd.h"
#include "render.h"

/* Encode a BGR565 pixel from 888 RGB values */
static uint16_t
rgb565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((b >> 3) << 11) | ((g >> 2) << 5) | (r >> 3);
}

int
main(int argc, char **argv)
{
	uint16_t *bitmap = (uint16_t *)calloc(X_RESOLUTION * Y_RESOLUTION, 2);
	if (!bitmap) {
		fprintf(stderr, "calloc failed\n");
		return 1;
	}

	/* Draw a solid red square: 100x100 pixels, centred */
	int sx = (X_RESOLUTION - 100) / 2; /* 270 */
	int sy = (Y_RESOLUTION - 100) / 2; /* 25 */
	uint16_t red = rgb565(255, 0, 0);

	for (int y = sy; y < sy + 100; y++)
		for (int x = sx; x < sx + 100; x++)
			bitmap[y * X_RESOLUTION + x] = red;

	/* Everything else is zero (transparent) */

	printf("Sending 100x100 red square at (%d,%d) as PARTIAL update...\n",
	       sx, sy);
	printf("Expected: red square appears on top of existing screen "
	       "content.\n");
	printf("          Rest of screen should be unchanged.\n");

	int rc = lcd_send_bitmap(bitmap, EFFECT_PARTIAL);
	if (rc != 0) {
		fprintf(stderr, "lcd_send_bitmap returned %d\n", rc);
		free(bitmap);
		return 1;
	}

	printf("Done.\n");
	free(bitmap);
	return 0;
}

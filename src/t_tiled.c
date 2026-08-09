/* Test: send a full-screen bitmap split into 4 tiled PARTIAL updates */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libspacelcd.h"
#include "render.h"

static uint16_t
rgb565(uint8_t r, uint8_t g, uint8_t b)
{
	return ((b >> 3) << 11) | ((g >> 2) << 5) | (r >> 3);
}

int
main(void)
{
	uint16_t *bitmap = (uint16_t *)calloc(X_RESOLUTION * Y_RESOLUTION, 2);
	if (!bitmap) {
		fprintf(stderr, "calloc failed\n");
		return 1;
	}

	/* Draw 4 coloured horizontal stripes, each ~38 pixels tall */
	uint16_t colours[] = {
		rgb565(255, 0, 0),   /* red    */
		rgb565(0, 255, 0),   /* green  */
		rgb565(0, 0, 255),   /* blue   */
		rgb565(255, 255, 0), /* yellow */
	};
	int stripe_h = Y_RESOLUTION / 4; /* 37-38 */

	for (int s = 0; s < 4; s++) {
		int y0 = s * stripe_h;
		int y1 = (s == 3) ? Y_RESOLUTION : (s + 1) * stripe_h;
		for (int y = y0; y < y1; y++)
			for (int x = 0; x < X_RESOLUTION; x++)
				bitmap[y * X_RESOLUTION + x] = colours[s];
	}

	printf("Sending 4-colour stripe bitmap as 4 tiled PARTIAL "
	       "updates...\n");
	printf("Expected: stripes appear top-to-bottom, one tile at a "
	       "time.\n");
	printf("          Final result: full screen of 4 coloured "
	       "stripes.\n");

	int rc = lcd_send_bitmap_tiled(bitmap, EFFECT_PARTIAL, 4);
	if (rc != 0) {
		fprintf(stderr, "lcd_send_bitmap_tiled returned %d\n", rc);
		free(bitmap);
		return 1;
	}

	printf("Done.\n");
	free(bitmap);
	return 0;
}

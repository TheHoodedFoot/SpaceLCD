/* Test: animate a red square moving left-to-right across the screen */
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

#define NUM_FRAMES 20
#define SQUARE_SIZE 50

int
main(void)
{
	uint16_t *frames[NUM_FRAMES];

	/* Pre-allocate all frames */
	for (int i = 0; i < NUM_FRAMES; i++) {
		frames[i] =
			(uint16_t *)calloc(X_RESOLUTION * Y_RESOLUTION, 2);
		if (!frames[i]) {
			fprintf(stderr, "calloc failed at frame %d\n", i);
			return 1;
		}
	}

	/* Generate frames: red square moves from left to right */
	int y = (Y_RESOLUTION - SQUARE_SIZE) / 2; /* centred vertically */
	uint16_t red = rgb565(255, 0, 0);

	for (int f = 0; f < NUM_FRAMES; f++) {
		/* Square position: moves across the screen */
		int x = (X_RESOLUTION / (NUM_FRAMES + 1)) * (f + 1) -
			SQUARE_SIZE / 2;
		if (x < 0) x = 0;
		if (x + SQUARE_SIZE > X_RESOLUTION)
			x = X_RESOLUTION - SQUARE_SIZE;

		for (int dy = 0; dy < SQUARE_SIZE; dy++)
			for (int dx = 0; dx < SQUARE_SIZE; dx++)
				frames[f][(y + dy) * X_RESOLUTION +
					  (x + dx)] = red;
	}

	printf("Animating red square left-to-right (%d frames at 15 "
	       "fps)...\n",
	       NUM_FRAMES);
	printf("Expected: smooth red square moving across the screen.\n");
	printf("          Duration: ~1.3 seconds.\n");

	int rc = lcd_animate((const uint16_t **)frames, NUM_FRAMES, 15,
			     EFFECT_PARTIAL);
	if (rc != 0) {
		fprintf(stderr, "lcd_animate returned %d\n", rc);
	} else {
		printf("Done.\n");
	}

	for (int i = 0; i < NUM_FRAMES; i++) free(frames[i]);
	return rc;
}

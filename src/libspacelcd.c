#include "libspacelcd.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "deflate.h"
#include "render.h"
#include "usb.h"

void
addheader(uint8_t *usbdata, int compressed_length, int effect)
{ /* Fill in header bytes for USB bulk data */

	usbdata[0] = effect;
	usbdata[1] = 0x0f;
	usbdata[2] = compressed_length;
	usbdata[3] = compressed_length >> 8;
	/* bytes 4-11 already zeroed by caller */
	/* Mode bytes 12-15: differ between FULL and PARTIAL */
	if (effect == EFFECT_PARTIAL) {
		usbdata[12] = 0x00;
		usbdata[13] = 0x00;
		usbdata[14] = 0x00;
		usbdata[15] = 0x0f;
	} else {
		usbdata[12] = 0x00;
		usbdata[13] = 0x3f;
		usbdata[14] = 0x01;
		usbdata[15] = 0xef;
	}
}

int
svgtolcd(const char *svg, int scroll)
{
	unsigned char *bitmap_buffer = (unsigned char *)malloc(BITMAP_BYTES);
	uint8_t *deflated_buffer =
		(uint8_t *)malloc(DEFLATED_MAX + SPLCD_HEADER_SIZE);
	memset(deflated_buffer, 0, SPLCD_HEADER_SIZE);

	/* Create bitmap from SVG */
	svgtorgb565(svg, strlen((const char *)svg), bitmap_buffer);

	/* Compress */
	int compressed_size =
		crush(bitmap_buffer, deflated_buffer + SPLCD_HEADER_SIZE,
		      BITMAP_BYTES);
	if (compressed_size < 0) {
		fprintf(stderr, "Failed to compress bitmap\n");
		free(deflated_buffer);
		free(bitmap_buffer);
		return 1;
	}

	if (compressed_size > 65535) {
		fprintf(stderr, "Compressed size too large\n");
		free(deflated_buffer);
		free(bitmap_buffer);
		return 1;
	}

	/* Generate header with compressed length */
	addheader(deflated_buffer, compressed_size, scroll);

	/* Send to USB */
	memorytousb(deflated_buffer, compressed_size + SPLCD_HEADER_SIZE, 0);

	free(deflated_buffer);
	free(bitmap_buffer);

	return 0;
}

int
svgtolcd_partial(const char *svg, int scroll)
{ /* Render SVG to bitmap, send as PARTIAL (alpha blend) update */
	/* scroll parameter is unused for PARTIAL; pass EFFECT_PARTIAL */

	unsigned char *bitmap_buffer = (unsigned char *)malloc(BITMAP_BYTES);
	uint8_t *deflated_buffer =
		(uint8_t *)malloc(DEFLATED_MAX + SPLCD_HEADER_SIZE);
	memset(deflated_buffer, 0, SPLCD_HEADER_SIZE);

	/* Create bitmap from SVG */
	svgtorgb565(svg, strlen((const char *)svg), bitmap_buffer);

	/* Compress */
	int compressed_size =
		crush(bitmap_buffer, deflated_buffer + SPLCD_HEADER_SIZE,
		      BITMAP_BYTES);
	if (compressed_size < 0) {
		fprintf(stderr, "svgtolcd_partial: compress failed\n");
		free(deflated_buffer);
		free(bitmap_buffer);
		return 1;
	}

	if (compressed_size > 65535) {
		fprintf(stderr,
			"svgtolcd_partial: compressed size %d exceeds "
			"65535\n",
			compressed_size);
		free(deflated_buffer);
		free(bitmap_buffer);
		return 1;
	}

	/* Generate header with PARTIAL effect */
	addheader(deflated_buffer, compressed_size, EFFECT_PARTIAL);

	/* Send to USB */
	memorytousb(deflated_buffer, compressed_size + SPLCD_HEADER_SIZE, 0);

	free(deflated_buffer);
	free(bitmap_buffer);

	return 0;
}

int
lcd_send_bitmap(const uint16_t *bitmap, int effect)
{ /* Send a 640x150 BGR565 bitmap to the LCD */
	/* effect: EFFECT_CUT (0x11) for full replace, EFFECT_PARTIAL (0x18)
	 * for alpha blend */

	uint8_t *deflated_buffer =
		(uint8_t *)malloc(DEFLATED_MAX + SPLCD_HEADER_SIZE);
	if (!deflated_buffer) {
		fprintf(stderr, "lcd_send_bitmap: malloc failed\n");
		return 1;
	}
	memset(deflated_buffer, 0, SPLCD_HEADER_SIZE);

	/* Convert uint16_t array to uint8_t array for compression */
	uint8_t *bitmap_bytes = (uint8_t *)bitmap;

	/* Compress */
	int compressed_size =
		crush(bitmap_bytes, deflated_buffer + SPLCD_HEADER_SIZE,
		      BITMAP_BYTES);
	if (compressed_size < 0) {
		fprintf(stderr, "lcd_send_bitmap: compress failed\n");
		free(deflated_buffer);
		return 1;
	}

	if (compressed_size > 65535) {
		fprintf(stderr,
			"lcd_send_bitmap: compressed size %d exceeds 65535\n",
			compressed_size);
		free(deflated_buffer);
		return 1;
	}

	/* Generate header */
	addheader(deflated_buffer, compressed_size, effect);

	/* Send to USB */
	memorytousb(deflated_buffer, compressed_size + SPLCD_HEADER_SIZE, 0);

	free(deflated_buffer);
	return 0;
}

int
lcd_send_bitmap_tiled(const uint16_t *bitmap, int effect, int num_tiles)
{ /* Send a bitmap split into horizontal tile updates */
	/* Each tile is a full 640x150 framebuffer with only one strip
	 * non-zero */
	/* Sent as PARTIAL updates so zeros are transparent */

	if (num_tiles <= 0) num_tiles = 1;

	int tile_height = Y_RESOLUTION / num_tiles;
	int remainder = Y_RESOLUTION % num_tiles;

	uint16_t *tile_bitmap =
		(uint16_t *)calloc(X_RESOLUTION * Y_RESOLUTION, 2);
	if (!tile_bitmap) {
		fprintf(stderr, "lcd_send_bitmap_tiled: malloc failed\n");
		return 1;
	}

	uint8_t *deflated_buffer =
		(uint8_t *)malloc(DEFLATED_MAX + SPLCD_HEADER_SIZE);
	if (!deflated_buffer) {
		free(tile_bitmap);
		fprintf(stderr, "lcd_send_bitmap_tiled: malloc failed\n");
		return 1;
	}

	int y_start = 0;
	for (int t = 0; t < num_tiles; t++) {
		/* Clear tile bitmap */
		memset(tile_bitmap, 0, X_RESOLUTION * Y_RESOLUTION * 2);

		/* Copy this tile's rows */
		int this_height = tile_height + (t < remainder ? 1 : 0);
		int y_end = y_start + this_height;

		for (int y = y_start; y < y_end; y++) {
			memcpy(tile_bitmap + y * X_RESOLUTION,
			       bitmap + y * X_RESOLUTION, X_RESOLUTION * 2);
		}

		/* Compress */
		memset(deflated_buffer, 0, SPLCD_HEADER_SIZE);
		int compressed_size = crush(
			(uint8_t *)tile_bitmap,
			deflated_buffer + SPLCD_HEADER_SIZE, BITMAP_BYTES);
		if (compressed_size < 0 || compressed_size > 65535) {
			fprintf(stderr,
				"lcd_send_bitmap_tiled: tile %d compress "
				"failed (%d)\n",
				t, compressed_size);
			free(tile_bitmap);
			free(deflated_buffer);
			return 1;
		}

		/* Use PARTIAL effect for alpha blending */
		addheader(deflated_buffer, compressed_size, EFFECT_PARTIAL);

		/* Send to USB */
		memorytousb(deflated_buffer,
			    compressed_size + SPLCD_HEADER_SIZE, 0);

		y_start = y_end;
	}

	free(tile_bitmap);
	free(deflated_buffer);
	return 0;
}

static void
usleep_exact(long usec)
{ /* Sleep for approximately usec microseconds */
	struct timespec ts;
	ts.tv_sec = usec / 1000000L;
	ts.tv_nsec = (usec % 1000000L) * 1000L;
	nanosleep(&ts, NULL);
}

int
lcd_animate(const uint16_t **bitmaps, int count, int fps, int effect)
{ /* Send an array of bitmaps as frames at the target FPS */
	/* Uses persistent USB connection to avoid per-frame open/reset/close
	 */

	if (count <= 0 || fps <= 0 || !bitmaps) {
		fprintf(stderr,
			"lcd_animate: invalid args (count=%d fps=%d)\n",
			count, fps);
		return 1;
	}

	long frame_interval_us = 1000000L / fps;
	uint8_t *deflated_buffer =
		(uint8_t *)malloc(DEFLATED_MAX + SPLCD_HEADER_SIZE);
	if (!deflated_buffer) {
		fprintf(stderr, "lcd_animate: malloc failed\n");
		return 1;
	}

	/* Open persistent USB connection */
	libusb_context *ctx = lcd_libusb_init();
	if (!ctx) {
		free(deflated_buffer);
		return 1;
	}

	libusb_device_handle *handle = lcd_open(ctx);
	if (!handle) {
		libusb_exit(ctx);
		free(deflated_buffer);
		return 1;
	}

	int rc = 0;
	for (int i = 0; i < count; i++) {
		struct timespec start;
		clock_gettime(CLOCK_MONOTONIC, &start);

		/* Compress */
		memset(deflated_buffer, 0, SPLCD_HEADER_SIZE);
		int compressed_size = crush(
			(uint8_t *)bitmaps[i],
			deflated_buffer + SPLCD_HEADER_SIZE, BITMAP_BYTES);
		if (compressed_size < 0 || compressed_size > 65535) {
			fprintf(stderr,
				"lcd_animate: frame %d compress failed "
				"(%d)\n",
				i, compressed_size);
			rc = 1;
			break;
		}

		/* Build header and send as single bulk transfer */
		addheader(deflated_buffer, compressed_size, effect);
		int total_size = compressed_size + SPLCD_HEADER_SIZE;
		if (lcd_send_bulk(handle, deflated_buffer, total_size)) {
			fprintf(stderr, "lcd_animate: frame %d send failed\n",
				i);
			rc = 1;
			break;
		}

		/* Calculate elapsed time and sleep to maintain frame rate */
		struct timespec end;
		clock_gettime(CLOCK_MONOTONIC, &end);
		long elapsed_us = (end.tv_sec - start.tv_sec) * 1000000L +
				  (end.tv_nsec - start.tv_nsec) / 1000L;
		long sleep_us = frame_interval_us - elapsed_us;
		if (sleep_us > 0) usleep_exact(sleep_us);
	}

	lcd_close(handle, ctx);
	free(deflated_buffer);
	return rc;
}

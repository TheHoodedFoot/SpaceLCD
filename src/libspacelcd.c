#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bgr.h"
#include "deflate.h"
#include "splcd.h"
#include "svg.hpp"
#include "usb.h"

int
svgtolcd(const char *svg, int scroll)
{
	unsigned char *bitmap_buffer = (unsigned char *)malloc(BITMAP_BYTES);
	uint8_t *deflated_buffer =
		(uint8_t *)malloc(DEFLATED_MAX + SPLCD_HEADER_SIZE);
	memset(deflated_buffer, 0, SPLCD_HEADER_SIZE);

	/* Create bitmap from SVG */
	svgtorgb565(svg, strlen((const char *)svg), (char *)bitmap_buffer);

	/* Convert to bgr */
	rgbtobgr(bitmap_buffer, BITMAP_BYTES);

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

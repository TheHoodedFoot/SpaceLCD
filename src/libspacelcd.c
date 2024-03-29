#include "libspacelcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deflate.h"
#include "render.h"
#include "usb.h"

void
addheader(uint8_t *usbdata, int compressed_length, int scroll)
{ /* Fill in header bytes for USB bulk data */

	usbdata[0] = scroll;
	usbdata[1] = 0x0f;
	usbdata[2] = compressed_length;
	usbdata[3] = compressed_length >> 8;
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

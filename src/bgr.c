#include "bgr.h"

void
addheader(uint8_t *usbdata, int compressed_length, int scroll)
{ /* Fill in header bytes for USB bulk data */

	*usbdata++ = scroll;
	*usbdata++ = 0x0f;
	*usbdata++ = compressed_length;
	*usbdata   = compressed_length >> 8;
}

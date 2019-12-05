#include "bgr.h"

void
rgbtobgr(uint8_t *buffer, int size)
{ /* Convert from R5G6B5 to B5G6R5 */

	for (int i = 0; i < size; i += 2) {
		uint8_t byte1 = *(buffer + i);
		uint8_t byte2 = *(buffer + i + 1);
		*(buffer + i) = (byte2 >> 3) | (byte1 & 0xe0);
		*(buffer + i + 1) = (byte1 << 3) | (byte2 & 0x07);
	}
}

void
swapendianness(uint8_t *bytes)
{
	uint8_t byte = *bytes;
	*(bytes) = *(bytes + 1);
	*(bytes + 1) = byte;
}

void
addheader(uint8_t *usbdata, int compressed_length, int scroll)
{ /* Fill in header bytes for USB bulk data */

	/* Endianness alert */
	*((uint16_t *)usbdata) = 0x0f << 8 | scroll;
	*((uint16_t *)usbdata + 1) = compressed_length;

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
	swapendianness(usbdata);
	swapendianness(usbdata + 2);
#endif
}

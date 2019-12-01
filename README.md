# Title

## Subtitle

**bold** main description

## Notes

USB (brightness)
	bmrequest: 0x21
	brequest: 9
	wvalue: 0x0311
	windex: 1
	wlength: 2
	data fragment: 1100 (off) 1107 (dim) 1160 (very bright) 1164 (max)

USB (change lcd graphics)
	URB_CONTROL
		bmrequest: 0x21
		brequest: 9
		wvalue: 0x0314
		windex: 1
		wlength: 5
		data fragment: 14fcff071c
	URB_BULK
		words are little-endian
		180f start of bulk header (512 bytes)
		next two bytes are size of payload following first 512 bytes (also byte 29/30)
		3e1a 2dff common
		110f complete redraw
		180f partial redraw of titles?
		1805 a360
USB_CONTROL
	0x0f,0x01 perhaps flips the display to show the alternate bitmap buffer

Main body after first 512 bytes:
	eb2b ee1b
		change ee1b to ee1c changes the grey colour
		then pixel data?

		28 1c 85 a3 70 14 8e c2 51 38 0a 47 e1
Splash screen:
	reset
	make (block_whole_screen_no_scroll)
	shutdown

To enable libusb debugging
	: libusb_set_option(NULL, LIBUSB_OPTION_LOG_LEVEL, * LIBUSB_LOG_LEVEL_DEBUG); 

The data may be compressed using gzip compression.

Create a bitmap, save as 565 rgb, dd skip=70 if=<file.bmp> bs=1 status=none | gzip -c

{ printf "\x1f\x8b\x08\x00\x00\x00\x00\x00\x00\x00"; dd skip=512 if=/tmp/<file> bs=1 status=none count=128; } > /tmp/file

## Bugs

## Example

~~~{.py}
import sys
~~~

## Copyright and Licencing

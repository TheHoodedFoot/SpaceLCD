#include <assert.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <libusb-1.0/libusb.h>
#include <zlib.h>

#include "libspacelcd.h"

#ifdef USE_PNG
#define PNG_DEBUG 3
#include <png.h>
#endif

#define USB_PACKET_MAX 64
#define USB_TIMEOUT 1000

#define SPNENTERPRISE_VENDOR_ID 0x256f
#define SPNENTERPRISE_PRODUCT_ID 0xc633

static int bulk_flag = 1, verbose_flag = 0;

void
claiminterfaces(libusb_device_handle *handle)
{
	for (int i = 0; i < 2; i++) {
		if (verbose_flag) printf("Claiming interface %d\n", i);
		int status = libusb_claim_interface(handle, i);
		if (status != LIBUSB_SUCCESS) {
			printf("Could not claim interface %d\n", i);
		}

		libusb_reset_device(handle);
	}
}

void
releaseinterfaces(libusb_device_handle *handle)
{
	/* We must release the interfaces to enable the kernel driver
	 * to reattach, otherwise spacenavd cannot get control back
	 * again until we hard reset. */
	for (int i = 0; i < 2; i++) {
		int status = libusb_release_interface(handle, i);
		if (status != LIBUSB_SUCCESS) {
			printf("Could not release interface %d\n", i);
		}
	}
}

int
sendbulk(libusb_device_handle *handle, unsigned char *bytes, int size)
{
	int transferred;
	if (verbose_flag) printf("Sending %d bytes (bulk)\n", size);
	libusb_bulk_transfer(handle, 0x01, bytes, size, &transferred,
			     USB_TIMEOUT);
	return transferred;
}

void
sendcontrol(libusb_device_handle *handle, unsigned char *bytes, int size,
	    int wvalue)
{
	if (verbose_flag)
		printf("Sending %d bytes (control) with wvalue %x\n", size,
		       wvalue);
	libusb_control_transfer(handle, 0x21, 0x09, wvalue, 1, bytes, size,
				USB_TIMEOUT);
}

int
filetousb(FILE *fhandle, int bulk, int wvalue)
{
	assert(fhandle);
	int r = libusb_init(NULL);
	if (r < 0) return r;

	libusb_device_handle *spnav_handle = libusb_open_device_with_vid_pid(
		NULL, SPNENTERPRISE_VENDOR_ID, SPNENTERPRISE_PRODUCT_ID);

	if (!spnav_handle) {
		printf("Failed to attach to spnav_handle. Check "
		       "permissions.\n");
		libusb_exit(NULL);
		return 0;
	}

	/* The kernel driver needs to be detached for our commands to
	 * work. This detaches and reattaches it automatically,
	 * but we must release the interface when we are done. */
	libusb_set_auto_detach_kernel_driver(spnav_handle, 1);

	claiminterfaces(spnav_handle);

	unsigned char usbdata[USB_PACKET_MAX + 1];
	int bytes_read;

	/* Read at most 64 bytes */
	while (((bytes_read = fread((char *)&usbdata, 1, USB_PACKET_MAX,
				    fhandle)))) {
		/* Send to bulk or contol as required */
		if (bulk)
			sendbulk(spnav_handle, (unsigned char *)&usbdata,
				 bytes_read);
		else
			sendcontrol(spnav_handle, (unsigned char *)&usbdata,
				    bytes_read, wvalue);
	}

	releaseinterfaces(spnav_handle);
	libusb_close(spnav_handle);
	libusb_exit(NULL);
	return 0;
}

int
isValidLCDData(uint8_t *lcddata)
{ /* Performs sanity check on bulk data block */

	/* Check lcd update type (0x00-0x01) in valid range */
	/* Calculate decompressed length; verify < 65536 */
	/* Ensure length field (0x02-0x03) matches decompressed length */

	return false;
}

int
svgToLCDData(uint8_t *svg, uint8_t *bulk, int scroll)
{ /* Convert an SVG string to a bulk data block */

	/* Allocate bitmap */
	/* Parse SVG > bitmap */
	/* Flip vertically */
	/* Convert to B5R6G5 */
	/* Compress */
	/* Fill in header fields */

	return false;
}

int
verticalFlip(uint8_t *bitmap)
{ /* Flip the bitmap vertically to match the SpaceExplorer orientation */

	return false;
}


#ifdef USE_PNG
int x, y;

int width, height;
png_byte color_type;
png_byte bit_depth;

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep *row_pointers;

void
read_png_file(char *file_name)
{
	char header[8];  // 8 is the maximum size that can be checked

	/* open file and test for it being a png */
	FILE *fp = fopen(file_name, "rb");
	if (!fp)
		abort_("[read_png_file] File %s could not be opened for "
		       "reading",
		       file_name);
	fread(header, 1, 8, fp);
	if (png_sig_cmp(header, 0, 8))
		abort_("[read_png_file] File %s is not recognized as a PNG "
		       "file",
		       file_name);


	/* initialize stuff */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
					 NULL);

	if (!png_ptr) abort_("[read_png_file] png_create_read_struct failed");

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
		abort_("[read_png_file] png_create_info_struct failed");

	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during init_io");

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);


	/* read file */
	if (setjmp(png_jmpbuf(png_ptr)))
		abort_("[read_png_file] Error during read_image");

	row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
	for (y = 0; y < height; y++)
		row_pointers[y] = (png_byte *)malloc(
			png_get_rowbytes(png_ptr, info_ptr));

	png_read_image(png_ptr, row_pointers);

	fclose(fp);
}

void
process_file(void)
{
	if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
		abort_("[process_file] input file is PNG_COLOR_TYPE_RGB but "
		       "must be PNG_COLOR_TYPE_RGBA "
		       "(lacks the alpha channel)");

	if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
		abort_("[process_file] color_type of input file must be "
		       "PNG_COLOR_TYPE_RGBA (%d) (is %d)",
		       PNG_COLOR_TYPE_RGBA,
		       png_get_color_type(png_ptr, info_ptr));

	for (y = 0; y < height; y++) {
		png_byte *row = row_pointers[y];
		for (x = 0; x < width; x++) {
			png_byte *ptr = &(row[x * 4]);
			printf("Pixel at position [ %d - %d ] has RGBA "
			       "values: %d - %d - %d - %d\n",
			       x, y, ptr[0], ptr[1], ptr[2], ptr[3]);

			/* set red value to 0 and green value to the blue one
			 */
			ptr[0] = 0;
			ptr[1] = ptr[2];
		}
	}
}

#endif


int
main(int argc, char **argv)
{
	int c, wvalue;

	while (1) {
		static struct option long_options[] = {
			/* These options set a flag. */
			{"verbose", no_argument, &verbose_flag, 1},
			{"bulk", no_argument, &bulk_flag, 1},
			{"control", no_argument, &bulk_flag, 0},
			{"help", no_argument, 0, 'h'},
			{"wvalue", no_argument, 0, 'w'},
			{0, 0, 0, 0}};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "bchvw:", long_options,
				&option_index);

		/* Detect the end of the options. */
		if (c == -1) break;

		switch (c) {
			case 0:
				/* If this option set a flag, do nothing else
				 * now. */
				if (long_options[option_index].flag != 0)
					break;
				printf("option %s",
				       long_options[option_index].name);
				if (optarg) printf(" with arg %s", optarg);
				printf("\n");
				break;

			case 'h':
				printf("%s: Send binary file to "
				       "SpaceExplorer\n",
				       argv[0]);
				puts("    -b bulk mode");
				puts("    -c control mode");
				puts("    -w control wvalue (0x0311 "
				     "brightness, 0x0314 other)");

			case 'v':
				verbose_flag = 1;
				break;

			case 'b':
				bulk_flag = 1;
				break;

			case 'c':
				bulk_flag = 0;
				break;

			case 'w':
				wvalue = (int)strtol(optarg, NULL, 16);
				break;

			case '?':
				/* getopt_long already printed an error
				 * message. */
				break;

			default:
				abort();
		}
	}

	if (optind < argc) {
		while (optind < argc) {
			FILE *fhandle;
			fhandle = fopen(argv[optind++], "r");
			if (!fhandle) {
				printf("Error opening file\n");
				return -1;
			}
			filetousb(fhandle, bulk_flag, wvalue);
			fclose(fhandle);
		}
	} else {
		filetousb(stdin, bulk_flag, wvalue);
	}

	return 0;
}

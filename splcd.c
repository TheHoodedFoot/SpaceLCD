#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "libusb.h"

#define USB_PACKET_MAX 64
#define SPACENAV_VENDOR_ID 0x256f
#define SPACENAV_PRODUCT_ID 0xc633

static int bulk_flag = 0, verbose_flag = 0;

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
	libusb_bulk_transfer(handle, 0x01, bytes, size, &transferred, 1000);
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
				1000);
}

int
filetousb(const char *filename, int bulk, int wvalue)
{
	FILE *fhandle;
	fhandle = fopen(filename, "r");
	if (!fhandle) {
		printf("Error opening file\n");
		return -1;
	}

	int r = libusb_init(NULL);
	if (r < 0) return r;

	libusb_device_handle *spnav_handle = libusb_open_device_with_vid_pid(
		NULL, SPACENAV_VENDOR_ID, SPACENAV_PRODUCT_ID);

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
		while (optind < argc)
			filetousb(argv[optind++], bulk_flag, wvalue);
	} else {
		printf("A filename is required.\n");
	}

	return 0;
}

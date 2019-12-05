#include <assert.h>
/* #include <stdbool.h> */
#include <stdio.h>
/* #include <stdlib.h> */
/* #include <string.h> */
/* #include <unistd.h> */

#include <libusb.h>
#include "usb.h"


void
claiminterfaces(libusb_device_handle *handle)
{
	for (int i = 0; i < 2; i++) {
		int status = libusb_claim_interface(handle, i);
		if (status != LIBUSB_SUCCESS) {
			fprintf(stderr, "Could not claim interface %d\n", i);
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
			fprintf(stderr, "Could not release interface %d\n",
				i);
		}
	}
}

int
sendbulk(libusb_device_handle *handle, unsigned char *bytes, int size)
{
	int transferred;
	libusb_bulk_transfer(handle, 0x01, bytes, size, &transferred,
			     USB_TIMEOUT);
	return transferred;
}

int
sendcontrol(libusb_device_handle *handle, unsigned char *bytes, int size,
	    int wvalue)
{
	return libusb_control_transfer(handle, 0x22, 0x09, wvalue, 1, bytes,
				       size, USB_TIMEOUT);
}

int
memorytousb(unsigned char *usbdata, int size, int wvalue)
{ /* Sends the memory to the SpaceExplorer */

	/* If wvalue is false then the message is bulk */

	int r = libusb_init(NULL);
	if (r < 0) return r;

	libusb_device_handle *spnav_handle = libusb_open_device_with_vid_pid(
		NULL, SPNENTERPRISE_VENDOR_ID, SPNENTERPRISE_PRODUCT_ID);

	if (!spnav_handle) {
		fprintf(stderr,
			"Failed to attach to spnav_handle. Check "
			"permissions.\n");
		libusb_exit(NULL);
		return 0;
	}

	/* The kernel driver needs to be detached for our commands to
	 * work. This detaches and reattaches it automatically,
	 * but we must release the interface when we are done. */
	libusb_set_auto_detach_kernel_driver(spnav_handle, 1);
	claiminterfaces(spnav_handle);

	int transferred;

	while (size > 0) {
		if (wvalue) {
			transferred = sendcontrol(
				spnav_handle, usbdata,
				size > USB_PACKET_MAX ? USB_PACKET_MAX : size,
				wvalue);
		} else {
			transferred = sendbulk(spnav_handle, usbdata,
					       size > USB_PACKET_MAX
						       ? USB_PACKET_MAX
						       : size);
			/* transferred = size > USB_PACKET_MAX ?
			 * USB_PACKET_MAX : size; */
			/* fwrite(usbdata, 1, size > USB_PACKET_MAX ?
			 * USB_PACKET_MAX : size, stdout); */
		}

		usbdata += transferred;
		size -= transferred;
	}

	releaseinterfaces(spnav_handle);
	libusb_close(spnav_handle);
	libusb_exit(NULL);
	return 1;
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
		fprintf(stderr,
			"Failed to attach to spnav_handle. Check "
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

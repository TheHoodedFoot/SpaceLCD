#ifndef USB_H_GXGA8TOL
#define USB_H_GXGA8TOL

#include <libusb.h>
#include <stdio.h>

#define USB_PACKET_MAX 64
#define USB_TIMEOUT 1000

#define SPNENTERPRISE_VENDOR_ID 0x256f
#define SPNENTERPRISE_PRODUCT_ID 0xc633

int filetousb(FILE *fhandle, int bulk, int wvalue);
int memorytousb(unsigned char *usbdata, int size, int wvalue);
int sendbulk(libusb_device_handle *handle, unsigned char *bytes, int size);
int sendcontrol(libusb_device_handle *handle, unsigned char *bytes, int size,
		int wvalue);
void claiminterfaces(libusb_device_handle *handle);
void releaseinterfaces(libusb_device_handle *handle);

/* Persistent connection API (for animation / rapid updates) */
libusb_context *lcd_libusb_init(void);
libusb_device_handle *lcd_open(libusb_context *ctx);
int lcd_send_bulk(libusb_device_handle *handle, unsigned char *data,
		  int size);
void lcd_close(libusb_device_handle *handle, libusb_context *ctx);

#endif /* end of include guard: USB_H_GXGA8TOL */

#include "render.h"

#include <librsvg/rsvg.h>

static void
rgbtobgr(guint8 *dst, guint8 *src, gsize size)
{ /* Convert from R5G6B5 to B5G6R5 */

	for (int i = 0; i < size; i += 2) {
		*dst++ = (src[i+1] >> 3) | (src[i]   & 0xe0);
		*dst++ = (src[i]   << 3) | (src[i+1] & 0x07);
	}
}


int
svgtorgb565(const char *svg, int size, void *buffer)
{ /* Parses SVG string and copies to supplied buffer */

	/* Buffer must be 192000 bytes in size */

	GError *error = NULL;
	RsvgHandle *handle;
	cairo_surface_t *surface;
	cairo_t *cr;

	handle = rsvg_handle_new_from_data((const guint8 *)svg, (gsize)size,
					   &error);
	if (error) return 1;

	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565,
					     BITMAP_WIDTH, BITMAP_HEIGHT);
	cr = cairo_create(surface);
	rsvg_handle_render_cairo(handle, cr);
	if (cairo_status(cr)) return 2;

	rgbtobgr((guint8 *)buffer, cairo_image_surface_get_data(surface), BITMAP_BYTES);

	cairo_destroy(cr);
	cairo_surface_destroy(surface);

	return 0;
}

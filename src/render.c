#include <stdlib.h>
#include <string.h>

#include "render.h"

#include <librsvg/rsvg.h>

int
svgtorgb565(const char *svg, int size, char *buffer)
{ /* Parses SVG string and copies to supplied buffer */

	/* Buffer must be 192000 bytes in size */

	GError *error = NULL;
	RsvgHandle *handle;
	cairo_surface_t *surface;
	cairo_t *cr;
	int mode = 1;

	handle = rsvg_handle_new_from_data((const guint8 *)svg, (gsize)size,
					   &error);
	if (error) return 1;

	surface = cairo_image_surface_create(CAIRO_FORMAT_RGB16_565,
					     BITMAP_WIDTH, BITMAP_HEIGHT);
	cr = cairo_create(surface);
	rsvg_handle_render_cairo(handle, cr);
	if (cairo_status(cr)) return 2;

	unsigned char *raw_buffer = cairo_image_surface_get_data(surface);
	memcpy(buffer, raw_buffer, BITMAP_BYTES);

	cairo_destroy(cr);
	cairo_surface_destroy(surface);

	return 0;
}

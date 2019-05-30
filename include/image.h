#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib.h>
#include <epoxy/gl.h>

#define G_MALLOC_ERROR  MallocError()

static inline GQuark MallocError(void) {
    return g_quark_from_static_string("Malloc Error");
}

guint8 *read_image_pixels(guint8 *pixels_buf, const gchar *filename, int *width, int *height, int *channels, int *stride, GError **error);
void free_image_pixels(guint8 *pixels);

#endif // IMAGE_H_INCLUDED

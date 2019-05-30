#include "image.h"
#include <string.h>
#include "gl_error.h"


guint8 *read_image_pixels(uint8_t *pixels_buf, const gchar *filename, int *width, int *height, int *channels, int *stride, GError **error) {
    GdkPixbuf *pixbuf;
    const guint8 *pixels;
    guint8 *result;

    pixbuf = gdk_pixbuf_new_from_file(filename, error);
    if (!pixbuf) {
        return NULL;
    }


    g_printerr("has_alpha: %d, stride %d\n", gdk_pixbuf_get_has_alpha(pixbuf), gdk_pixbuf_get_rowstride(pixbuf));

    *width = gdk_pixbuf_get_width(pixbuf);
    *height = gdk_pixbuf_get_height(pixbuf);
    *channels = gdk_pixbuf_get_n_channels(pixbuf);
    *stride = gdk_pixbuf_get_rowstride(pixbuf);

    pixels = gdk_pixbuf_read_pixels(pixbuf);

    if (!pixels_buf)
        result = g_malloc0(*height * *stride);
    else
        result = pixels_buf;
    if (result) {
        result = memcpy(result, pixels, *height * *stride);
    } else {
        if (error)
            *error = g_error_new(G_MALLOC_ERROR, 1, "Failed to malloc memory");
    }

    g_object_unref(pixbuf);
    return result;
}


void free_image_pixels(guint8 *pixels)
{
    g_free(pixels);
}

#include "image.h"
#include "gl_error.h"
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <string.h>


uint8_t *read_image_pixels(uint8_t *pixels_buf, const char *filename, int *width, int *height, int *channels, int *stride) {
    GdkPixbuf *pixbuf;
    const uint8_t *pixels;
    uint8_t *result;
    GError *error = NULL;

    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if (!pixbuf) {
        g_printerr("%s: failed to create pixbuf: %s\n", __func__, error->message);
        g_error_free(error);
        return NULL;
    }

    *width = gdk_pixbuf_get_width(pixbuf);
    *height = gdk_pixbuf_get_height(pixbuf);
    *channels = gdk_pixbuf_get_n_channels(pixbuf);
    *stride = gdk_pixbuf_get_rowstride(pixbuf);

    pixels = gdk_pixbuf_read_pixels(pixbuf);

    result = pixels_buf;
    if (!result) {
        result = g_malloc0(*height * *stride);
        g_printerr("%s: failed to alloc memory\n", __func__);
        return NULL;
    }

    result = memcpy(result, pixels, *height * *stride);
    g_object_unref(pixbuf);
    return result;
}


void free_image_pixels(uint8_t *pixels)
{
    g_free(pixels);
}

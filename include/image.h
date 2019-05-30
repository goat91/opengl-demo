#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <stdint.h>

uint8_t *read_image_pixels(
		uint8_t *pixels_buf,
		const char *filename,
		int *width,
		int *height,
		int *channels,
		int *stride);
void free_image_pixels(uint8_t *pixels);

#endif // IMAGE_H_INCLUDED

#ifndef GLF_IMAGE_H
#define GLF_IMAGE_H

#include <stdint.h>

/**
 * @brief The image struct
 */
typedef struct glf_image
{
	uint32_t width;
	uint32_t height;
	uint32_t bpp;
	uint8_t *data; /**< Color data */
} glf_image;

/**
 * @brief Load an image from file
 */
extern glf_image *glf_image_load(const char *filename);

/**
 * @brief Destroy image
 */
extern void glf_image_destroy(glf_image *img);

#endif

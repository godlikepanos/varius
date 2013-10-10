#ifndef GLF_TEXTURE_H
#define GLF_TEXTURE_H

#include "glf/common.h"
#include "glf/gl.h"

/**
 * @brief XXX
 */
struct glf_texture_initializer {
	uint32_t       width;
	uint32_t       height;
	GLenum         internal_format;
	GLenum         format;
	GLenum         type;
	const void    *data;
	uint8_t        mipmapping; /**< Gen mipmaps? */
	uint8_t        filter;
	uint8_t        repeat;
	int            anisotropy;
	size_t         data_size; /**< For compressed textures */
};

/**
 * @brief Texture structure
 */
struct glf_texture {
	GLuint id;
};

/**
 * @brief Create 2D texture
 */
extern struct glf_texture *
glf_texture_create_2d(const struct glf_texture_initializer *init);

/**
 * @brief Create cube texture
 */
extern struct glf_texture *
glf_texture_create_cube(const struct glf_texture_initializer *init[4]);

/**
 * @brief XXX
 */
extern struct glf_texture *glf_texture_create_from_file(const char *filename);

/**
 * @brief XXX
 */
extern void glf_texture_destroy(struct glf_texture *tex);

#endif

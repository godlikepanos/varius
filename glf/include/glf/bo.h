/**
 * @file bo.h
 * @brief Includes the OpenGL buffer object code
 */

#ifndef GLF_BO_H
#define GLF_BO_H

#include "glf/common.h"
#include "glf/gl.h"

/**
 * @brief Struct that initializes the buffer object.
 */
struct glf_bo_initializer {
	GLenum      target;
	uint32_t    size;
	const void *data;
	GLenum      usage;
};

/**
 * @brief Buffer object.
 */
struct glf_bo {
	GLuint id;
	GLenum target;
	GLenum usage;
	size_t size; /**< In bytes */
} glf_bo;

/**
 * @brief Create new buffer object
 */
extern struct glf_bo *glf_bo_create(const struct glf_bo_initializer *init);

/**
 * @brief Destroy a buffer object
 */
extern void glf_bo_destroy(struct glf_bo *bo);

/**
 * @brief Bind the BO
 */
extern void glf_bo_bind(struct glf_bo *bo);

#endif

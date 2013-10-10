#ifndef GLF_VAO_H
#define GLF_VAO_H

#include "glf/common.h"
#include "glf/gl.h"

/* Forward */
struct glf_bo;

/**
 * @brief The VAO representation
 */
struct glf_vao
{
	GLuint id;
};

/**
 * @brief Create a VAO.
 */
extern struct glf_vao *glf_vao_create();

/**
 * @brief Destroy a VAO.
 */
extern void glf_vao_destroy(struct glf_vao *vao);

/**
 * @brief Attach index array buffer.
 */
extern void glf_vao_attach_element_bo(struct glf_vao *vao, struct glf_bo *bo);

/**
 * @brief Attach a VBO.
 */
extern void glf_vao_attach_array_bo(struct glf_vao *vao,
									struct glf_bo *bo,
									GLuint attib_location,
									GLint size,
									GLenum type,
									GLboolean normalized,
									GLsizei stride,
									const GLvoid *pointer);

/**
 * @brief Bind the VAO
 */
void glf_vao_bind(const glf_vao *vao);

#endif

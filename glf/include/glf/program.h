#ifndef GLF_PROGRAM_H
#define GLF_PROGRAM_H

#include "glf/common.h"
#include "glf/gl.h"

#if defined(GLF_GL_VERSION_GLES1)
#	error "GLES 1.x does not support programs. Don't include this file"
#endif

/**
 * @brief Shader program variable
 */
struct glf_shader_variable {
	GLint  id;
	char  *name;
	GLenum type;
};

/**
 * @brief The representation of the program
 */
struct glf_program
{
	GLuint name;
	GLuint vert;
	GLuint frag;

	struct glf_shader_variable *vars;
	uint32_t		    vars_num;
};

/**
 * @brief Create a new program
 */
extern struct glf_program *glf_program_create(const char *vert, const char *tc,
					      const char *te, const char *geom,
					      const char *frag);

/**
 * @brief Create a new program with vertex and fragment shaders only
 */
extern struct glf_program *glf_program_create_vf(const char *vert,
						 const char *frag);

/**
 * @brief Load a new program with vertex and fragment shaders only
 */
extern struct glf_program *glf_program_load_vf(const char *vert_filename,
					       const char *frag_filename);

/**
 * @brief Use the program
 */
extern void glf_program_use(const struct glf_program *prog);

/**
 * @brief Get attribute location
 *
 * @return -1 on failure
 */
extern GLint glf_attribute_get_location(const struct glf_program *prog,
					const char *name);

/**
 * @brief Get uniform location
 *
 * @return -1 on failure
 */
extern GLint glf_uniform_get_location(const struct glf_program *prog,
				      const char *name);


#endif

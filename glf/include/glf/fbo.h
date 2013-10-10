#ifndef GLF_FBO_H
#define GLF_FBO_H

#include "glf/gl.h"

/**
 * @brief FBO representation
 */
struct glf_fbo {
	GLuint  id;
};

/**
 * @brief Create an FBO.
 */
extern struct glf_fbo *glf_fbo_create();

/**
 * @brief Destroy an FBO.
 */
extern void glf_fbo_destroy(struct glf_fbo *fbo);

/**
 * @brief Bind an FBO.
 */
extern void glf_fbo_bind(const struct glf_fbo *fbo, GLenum target);

/**
 * @brief Check if the FBO is OK
 * The function is called when we attach an FBO to the current state
 * @return 0 if OK
 */
extern int glf_fbo_check_current_completeness(GLenum target);

#endif

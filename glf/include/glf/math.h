#ifndef GLF_MATH_H
#define GLF_MATH_H

#include "glf/common.h"
#include <math.h>

#define GLF_PI 3.14159265

/**
 * @brief Set matrix to identity
 */
extern void glf_mat_set_identity(float *dest);

/**
 * @brief Set matrix to perspective projection
 */
extern void glf_mat_set_projection(float fovx, float fovy, float znear, 
				   float zfar, float *dest);

/**
 * @brief Set translation part
 */
extern void glf_mat_set_translation(const float *vec4, float *dest);

/**
 * @brief Set rotation part
 *
 * @param[in] x X angle
 * @param[in] y Y angle
 * @param[in] z Y angle
 * @param[out] m The output matrix
 */
extern void glf_mat_set_rotation(float x, float y, float z, float *dest);

/**
 * @brief Multiply matrix by matrix
 */
extern void glf_mat_mul(const float *a, const float *b, float *dest);

/**
 * @brief Invert matrix
 */
extern void glf_mat_invert(const float *in, float *dest);

/**
 * @brief Transpose matrix
 */
extern void glf_mat_transpose(const float *in, float *dest);

/**
 * @brief For debugging
 */
extern void glf_mat_print(const float *in);

#define GLF_TO_RAD(degrees) (degrees * (GLF_PI / 180.0))

#endif

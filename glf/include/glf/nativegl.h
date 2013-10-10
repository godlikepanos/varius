#ifndef GLF_NATIVE_GL_H
#define GLF_NATIVE_GL_H

#include "glf/common.h"

/**
 * @brief Window initializer
 */
typedef struct glf_window_initializer
{
	int bits_rgba[4];
	int samples_num;
	int depth_size;
	int stencil_size;
	int width;
	int height;
	const char *title;
} glf_window_initializer;

/**
 * @brief Create a new window
 */
extern struct glf_window *glf_window_create(const glf_window_initializer *i);

/**
 * @brief Destroy a window
 */
extern void glf_window_destroy(struct glf_window *w);

/**
 * @brief Draw callback
 */
typedef int (*glf_draw_callback)(void*);

/**
 * @brief The parameters of the main loop
 */
typedef struct glf_main_loop
{
	glf_draw_callback draw_callback;
	void *draw_callback_parameters;

	int timer_tick; /**< XXX */
	struct glf_window* window;
} glf_main_loop;

/**
 * @brief Main loop
 * 
 * @return 0 if no error
 */
extern int glf_main_loop_run(glf_main_loop *m);

#endif

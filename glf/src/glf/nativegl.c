/**
 * @file
 * The file contains common code for all native windows
 */

#include "glf/nativegl.h"
#include "glf/time.h"

extern void glf_swap_buffers(struct glf_window *w);
extern void glf_handle_events(glf_main_loop *m);

int glf_main_loop_run(glf_main_loop *m)
{
	int err;
	struct glf_window *w = m->window;
	(void)w;

	while(1)
	{
		uint32_t time = glf_get_ticks();
		int elapsed, delayt;

		glf_handle_events(m);

		err = m->draw_callback(m->draw_callback_parameters);
		if(err)
		{
			break;
		}

		glf_swap_buffers(m->window);

		/* Sleep */
		elapsed = glf_get_ticks() - time;
		delayt = m->timer_tick - elapsed;
		if(delayt > 0)
		{
			glf_delay(delayt);
		}
	}

	return err;
}

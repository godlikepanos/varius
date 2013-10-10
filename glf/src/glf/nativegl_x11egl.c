#include "glf/nativegl.h"
#include "glf/log.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <EGL/egl.h>

/**
 * @brief The window representation
 */
typedef struct glf_window
{
	Display *x_display;
	Window   x_window;
	EGLDisplay egl_display;
	EGLContext egl_context;
	EGLSurface egl_surfase;
	EGLSurface egl_config;
} glf_window;

/**
 * @brief Chose an EGL config
 */
static int glf_chose_config(const glf_window_initializer *init, glf_window *w)
{
	EGLint attribs[256];
	int attr = 0;
	int max_configs;
	EGLint num_configs;

	assert(init);
	assert(w);

	/*
	 * Attribs
	 */
	attribs[attr++] = EGL_RENDERABLE_TYPE;
#if defined(GLF_GL_VERSION_GLES1)
	attribs[attr++] = EGL_OPENGL_ES_BIT;
#elif defined(GLF_GL_VERSION_GLES2)
	attribs[attr++] = EGL_OPENGL_ES2_BIT;
#else
#	error "See file"
#endif

	attribs[attr++] = EGL_RED_SIZE;
	attribs[attr++] = init->bits_rgba[0];
	attribs[attr++] = EGL_GREEN_SIZE;
	attribs[attr++] = init->bits_rgba[1];
	attribs[attr++] = EGL_BLUE_SIZE;
	attribs[attr++] = init->bits_rgba[2];

	if(init->bits_rgba[3])
	{
		attribs[attr++] = EGL_ALPHA_SIZE;
		attribs[attr++] = init->bits_rgba[3];
	}

	if(init->depth_size)
	{
		attribs[attr++] = EGL_DEPTH_SIZE;
		attribs[attr++] = init->depth_size;
	}

	if(init->stencil_size)
	{
		attribs[attr++] = EGL_STENCIL_SIZE;
		attribs[attr++] = init->stencil_size;
	}

	if(init->samples_num > 0)
	{
		attribs[attr++] = EGL_SAMPLES;
		attribs[attr++] = init->samples_num;
	}

	attribs[attr++] = EGL_NONE;

	/*
	 * Continue
	 */
	if(!eglGetConfigs(w->egl_display, NULL, 0, &max_configs))
	{
		GLF_LOGE("eglGetConfigs() failed");
		return 1;
	}

	if(max_configs < 1)
	{
		GLF_LOGE("max_configs < 1");
		return 1;
	}

	if(!eglChooseConfig(w->egl_display, attribs, &w->egl_config, 1, 
	                    &num_configs))
	{
		GLF_LOGE("eglChooseConfig() failed");
		return 1;
	}

	assert(w->egl_config);
	assert(num_configs > 0);

	return 0;
}

/**
 * @brief Create context and surface
 */
static int glf_create_context_and_surface(glf_window *w)
{
	EGLint val;
	static const EGLint ctx_attribs[] = {
#if defined(GLF_GL_VERSION_GLES1)
		EGL_CONTEXT_CLIENT_VERSION, 1,
#elif defined(GLF_GL_VERSION_GLES2)
		EGL_CONTEXT_CLIENT_VERSION, 2,
#else
#	error "See file"
#endif
		EGL_NONE};

	assert(w);

	/*eglBindAPI(EGL_OPENGL_ES_API);*/

	/* Ctx */
	w->egl_context = eglCreateContext(w->egl_display, w->egl_config, 
	                                  EGL_NO_CONTEXT, ctx_attribs);
	if(!w->egl_context) 
	{
		GLF_LOGE("eglCreateContext() failed");
		return 1;
	}

	/* test eglQueryContext() */	
	eglQueryContext(w->egl_display, w->egl_context,
	                EGL_CONTEXT_CLIENT_VERSION, &val);
#if defined(GLF_GL_VERSION_GLES1)
	assert(val == 1);
#elif defined(GLF_GL_VERSION_GLES2)
	assert(val == 2);
#else
#	error "See file"
#endif

	/* Surf */
	w->egl_surfase = eglCreateWindowSurface(w->egl_display, 
	                                        w->egl_config, w->x_window, NULL);

	if(!w->egl_surfase)
	{
		GLF_LOGE("eglCreateWindowSurface() failed");
		return 1;
	}

	return 0;
}

/**
 * @brief Do some post sanity checks
 */
static int glf_do_post_sanity_checks(const glf_window_initializer *init,
                                     glf_window *w)
{
	EGLint val;

	assert(init);
	assert(w);

	eglQuerySurface(w->egl_display, w->egl_surfase, EGL_WIDTH, &val);
	assert(val == init->width);
	eglQuerySurface(w->egl_display, w->egl_surfase, EGL_HEIGHT, &val);
	assert(val == init->height);
	assert(eglGetConfigAttrib(w->egl_display, w->egl_config, EGL_SURFACE_TYPE, 
							  &val));
	assert(val & EGL_WINDOW_BIT);

	return 0;
}

/**
 * XXX
 */
static int glf_create_native_window(const glf_window_initializer *init, 
                                    glf_window *w)
{
	int scrnum;
	Window root;
	unsigned long mask;
	XSetWindowAttributes xattr;
	XVisualInfo *vis_info, vis_template;
	EGLint vid;
	int num_visuals;

	assert(init);
	assert(w);

	assert(w->x_display);
	scrnum = DefaultScreen(w->x_display);
	root = RootWindow(w->x_display, scrnum);

	if(!eglGetConfigAttrib(w->egl_display, w->egl_config,
	                       EGL_NATIVE_VISUAL_ID, &vid)) 
	{
		GLF_LOGE("eglGetConfigAttrib() failed");
		return 1;
	}
	
	/* The X window visual must match the EGL config */
	vis_template.visualid = vid;
	vis_info = XGetVisualInfo(w->x_display, VisualIDMask, &vis_template, 
	                          &num_visuals);
	if(!vis_info) 
	{
		GLF_LOGE("XGetVisualInfo() failed");
		return 1;
	}

	/* window attributes */
	xattr.background_pixel = 0;
	xattr.border_pixel = 0;
	xattr.colormap = XCreateColormap(w->x_display, root, vis_info->visual, 
	                                 AllocNone);
	xattr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
	mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	w->x_window = XCreateWindow(w->x_display, root, 0, 0, init->width, 
	                            init->height, 0, vis_info->depth, InputOutput,
	                            vis_info->visual, mask, &xattr);

	/* set hints and properties */
	{
		XSizeHints sizehints;
		sizehints.x = 0;
		sizehints.y = 0;
		sizehints.width  = init->width;
		sizehints.height = init->height;
		sizehints.flags = USSize | USPosition;
		XSetNormalHints(w->x_display, w->x_window, &sizehints);
		XSetStandardProperties(w->x_display, w->x_window, init->title, 
		                       init->title, None, (char **)NULL, 0, 
		                       &sizehints);
	}

	XMapWindow(w->x_display, w->x_window);
	XFree(vis_info);

	return 0;
}

/**
 * @brief XXX
 */
static int glf_create_egl_display(glf_window *w)
{
	w->x_display = XOpenDisplay(NULL);
	if(!w->x_display)
	{
		GLF_LOGE("XOpenDisplay() failed");
		return 1;
	}

	w->egl_display = eglGetDisplay(w->x_display);

	if(!w->egl_display)
	{
		GLF_LOGE("eglGetDisplay() failed");
		return 1;
	}

	return 0;
}

glf_window *glf_window_create(const glf_window_initializer *init)
{
	glf_window *w;
	int minor, major;
	int err = 0;

	w = (glf_window*)malloc(sizeof(glf_window));
	if(!w)
	{
		goto exit;
	}
	memset(w, 0, sizeof(glf_window));

	/* EGL display */
	err = glf_create_egl_display(w);
	if(err)
	{
		goto clean_window;
	}

	/* EGL init */
	if(!eglInitialize(w->egl_display, &major, &minor))
	{
		GLF_LOGE("eglInitialize() failed");
		goto clean_window;
	}

	/* EGL config */
	err = glf_chose_config(init, w);
	if(err)
	{
		goto clean_window;
	}

	/* Window */
	err = glf_create_native_window(init, w);
	if(err)
	{
		goto clean_window;
	}

	/* EGL context */
	err = glf_create_context_and_surface(w);
	if(err)
	{
		goto clean_window;
	}

	if(!eglMakeCurrent(w->egl_display, w->egl_surfase, w->egl_surfase, 
	                   w->egl_context))
	{
		GLF_LOGE("eglMakeCurrent() failed");
		goto clean_window;
	}

	/* EGL sanity */
	err = glf_do_post_sanity_checks(init, w);
	if(err)
	{
		goto clean_window;
	}

	/* All OK */
	return w;

	/* Cleanup */
clean_window:
	free(w);
exit:
	return NULL;
}

void glf_window_destroy(glf_window *w)
{
	if(!w)
	{
		return;
	}

	if(w->egl_context && w->egl_display)
	{
		eglDestroyContext(w->egl_display, w->egl_context);
	}

	if(w->egl_display && w->egl_surfase)
	{
		eglDestroySurface(w->egl_display, w->egl_surfase);
	}

	if(w->egl_display)
	{
		eglTerminate(w->egl_display);
	}

	if(w->x_display && w->x_window)
	{
		XDestroyWindow(w->x_display, w->x_window);
	}

	if(w->x_display)
	{
		XCloseDisplay(w->x_display);
	}
}

/**
 * @brief Self explanatory
 */
void glf_swap_buffers(glf_window *w)
{
	assert(w);
	eglSwapBuffers(w->egl_display, w->egl_surfase);
}

/**
 * @brief Self explanatory
 */
void glf_handle_events(glf_main_loop *m)
{
	XEvent event;

	assert(m);

	XNextEvent(m->window->x_display, &event);
}

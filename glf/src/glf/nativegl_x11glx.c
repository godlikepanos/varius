#include "glf/nativegl.h"
#include "glf/log.h"
#include "glf/gl.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <GL/glx.h>

extern int glf_gl_init();

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, 
                                     GLXFBConfig, GLXContext, Bool, const int*);

/**
 * @brief The window representation
 */
typedef struct glf_window
{
	Display *x_display;
	Window   x_window;
	Colormap x_colormap; /**< Its is actualy an unsigned int */
	GLXContext glx_context; /**< Its actualy a pointer */
	GLXFBConfig *glx_config;
} glf_window;

/**
 * XXX
 */
static int glf_create_display(glf_window *w)
{
	assert(w);

	w->x_display = XOpenDisplay(0);

	if(!w->x_display)
	{
		GLF_LOGE("XOpenDisplay() failed");
		return 1;
	}

	return 0;
}

/**
 * XXX
 */
static int glf_create_config(const glf_window_initializer * init, 
                             glf_window *w)
{
	int attribs[256];
	int at = 0;
	int fbcount;

	assert(init && w);

	attribs[at++] = GLX_X_RENDERABLE;
	attribs[at++] = True;

	attribs[at++] = GLX_DRAWABLE_TYPE;
	attribs[at++] = GLX_WINDOW_BIT;

	attribs[at++] = GLX_RENDER_TYPE;
	attribs[at++] = GLX_RGBA_BIT;

	attribs[at++] = GLX_X_VISUAL_TYPE;
	attribs[at++] = GLX_TRUE_COLOR;

	attribs[at++] = GLX_RED_SIZE;
	attribs[at++] = (init->bits_rgba[0]) ? init->bits_rgba[0] : 8;

	attribs[at++] = GLX_GREEN_SIZE;
	attribs[at++] = (init->bits_rgba[1]) ? init->bits_rgba[1] : 8;

	attribs[at++] = GLX_GREEN_SIZE;
	attribs[at++] = (init->bits_rgba[2]) ? init->bits_rgba[2] : 8;

	attribs[at++] = GLX_ALPHA_SIZE;
	attribs[at++] = (init->bits_rgba[3]) ? init->bits_rgba[3] : 8;

	attribs[at++] = GLX_DEPTH_SIZE;
	attribs[at++] = (init->depth_size) ? init->depth_size : 24;

	attribs[at++] = GLX_STENCIL_SIZE;
	attribs[at++] = (init->stencil_size) ? init->stencil_size : 8;

	attribs[at++] = GLX_DOUBLEBUFFER;
	attribs[at++] = True;

	if(init->samples_num)
	{
		attribs[at++] = GLX_SAMPLE_BUFFERS;
		attribs[at++] = 1;

		attribs[at++] = GLX_SAMPLES;
		attribs[at++] = init->samples_num;
	}

	attribs[at++] = None;

	w->glx_config = glXChooseFBConfig(w->x_display,
	                                  DefaultScreen(w->x_display), 
	                                  attribs, &fbcount);

	if(!w->glx_config)
	{
		GLF_LOGE("glXChooseFBConfig() failed");
		return 1;
	}

	return 0;
}

/**
 * XXX
 */
static int glf_create_native_window(const glf_window_initializer *init, 
                                    glf_window *w)
{
	XVisualInfo *vi;
	XSetWindowAttributes swa;
	Window root;

	assert(init && w);

	vi = glXGetVisualFromFBConfig(w->x_display, w->glx_config[0]);

	if(!vi)
	{
		GLF_LOGE("XGetVisualInfo() failed");
		return 1;
	}

	root = RootWindow(w->x_display, vi->screen);

	/* creating colormap */
	swa.colormap = w->x_colormap = XCreateColormap(w->x_display, root, 
	                                               vi->visual, AllocNone);
	swa.background_pixmap = None ;
	swa.border_pixel      = 0;
	swa.event_mask        = StructureNotifyMask;
 
	w->x_window = XCreateWindow(w->x_display, root, 
	                            0, 0, init->width, init->height, 0, 
	                            vi->depth, InputOutput, vi->visual, 
	                            CWBorderPixel | CWColormap | CWEventMask, 
	                            &swa);

	XFree(vi);

	if(!w->x_window)
	{
		GLF_LOGE("XCreateWindow() failed");
		return 1;
	}

	XStoreName(w->x_display, w->x_window, init->title);
	XMapWindow(w->x_display, w->x_window);

	return 0;
}

/**
 * @brief Create the context
 */
static int glf_create_context(glf_window *w)
{
	int ctx_attribs[] = {
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		None};

	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;

	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
	       glXGetProcAddressARB((const GLubyte *) "glXCreateContextAttribsARB");

	if(!glXCreateContextAttribsARB)
	{
		GLF_LOGE("Cannot get addres of proc glXCreateContextAttribsARB");
		return 1;
	}
	
	w->glx_context = glXCreateContextAttribsARB(w->x_display, *w->glx_config, 0,
	                                            True, ctx_attribs);

	XSync(w->x_display, False);

	if(!w->glx_context)
	{
		GLF_LOGE("glXCreateContextAttribsARB() failed");
		return 1;
	}

	if(!glXMakeCurrent(w->x_display, w->x_window, w->glx_context))
	{
		GLF_LOGE("glXMakeCurrent() failed");
		return 1;
	}

	return 0;
}

glf_window *glf_window_create(const glf_window_initializer *init)
{
	glf_window *w;
	int minor, major;

	/* alloc */
	w = (glf_window*)malloc(sizeof(glf_window));
	if(!w)
	{
		goto exit;
	}
	memset(w, 0, sizeof(glf_window));

	/* display */
	if(glf_create_display(w))
	{
		goto destroy_window;
	}

	/* GLX init */
	if(!glXQueryVersion(w->x_display, &major, &minor))
	{
		GLF_LOGE("glXQueryVersion() failed");
		goto destroy_window;
	}

	if(((major == 1) && (minor < 3)) || (major < 1))
	{
		GLF_LOGE("Old GLX version");
		goto destroy_window;
	}

	/* config */
	if(glf_create_config(init, w))
	{
		goto destroy_window;
	}

	/* native window */
	if(glf_create_native_window(init, w))
	{
		goto destroy_window;
	}

	/* context */
	if(glf_create_context(w))
	{
		goto destroy_window;
	}

	/* GL */
	/*if(glf_gl_init())*/
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		GLF_LOGE("GL initialization failed");
		goto destroy_window;
	}
	glGetError();

	return w;

destroy_window:
	glf_window_destroy(w);
exit:
	return NULL;
}

/**
 * @brief Self explanatory
 */
void glf_swap_buffers(glf_window *w)
{
	assert(w);
	glXSwapBuffers(w->x_display, w->x_window);
}

void glf_window_destroy(glf_window *w)
{
	assert(w);

	if(w->glx_context)
	{
		assert(w->x_display);
		glXMakeCurrent(w->x_display, 0, 0);
		glXDestroyContext(w->x_display, w->glx_context);
	}

	if(w->x_window)
	{
		assert(w->x_display);
		XDestroyWindow(w->x_display, w->x_window);
	}

	if(w->x_colormap)
	{
		assert(w->x_display);
		XFreeColormap(w->x_display, w->x_colormap);
	}

	if(w->x_display)
	{
		XCloseDisplay(w->x_display);
	}

	free(w);
}

/**
 * @brief Self explanatory
 */
void glf_handle_events(glf_main_loop *m)
{
	assert(m);
	(void)m;
}

#ifndef GLF_GL_H
#define GLF_GL_H

#include "glf/common.h"

#if defined(GLF_GL_VERSION_GLES1)
#	include <GLES/gl.h>
#elif defined(GLF_GL_VERSION_GLES2)
#	include <GLES2/gl2.h>
#elif defined(GLF_GL_VERSION_GL3)
/*#	include <GL/glx.h>
#	include <GL/glxext.h>*/
#	include <GL/glew.h>
#	if !defined(GLF_GLEW_H)
#		error "Wrong GLEW included"
#	endif
#else
#	error "See file"
#endif

extern void glf_check_gl_error(const char *file, int line, const char *func);
#define GLF_CHECK_GL_ERROR() glf_check_gl_error(__FILE__, __LINE__, \
                                                __FUNCTION__)

/**
 * @brief Read the framebuffer and save it to TGA
 *
 * @param[in] filename Where to save the file
 * @return 0 on success
 */
extern int glf_take_screenshot(const char* filename);

/*
 * Extra GL functions
 */
#if defined(GLF_GL_VERSION_GL3_XXX)
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
#endif

#endif


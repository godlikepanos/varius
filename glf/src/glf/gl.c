#include "glf/gl.h"
#include "glf/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void glf_check_gl_error(const char *file, int line, const char *func)
{
	GLenum err = glGetError();

	assert(file && func);

	if(err != GL_NO_ERROR)
	{
		glf_log_printf(file, line, func, GLF_LMT_ERROR, 
		               "glGetError() returned 0x%x", err);
		exit(err);
	}
}

int glf_take_screenshot(const char* filename)
{
	FILE *file;
	const uint8_t tga_header_uncompressed[12] = {
		0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t header[6];
	char *buffer;
	uint32_t i;
	size_t img_size;
	GLint viewport[4];

	assert(filename);

	glGetIntegerv(GL_VIEWPORT, &viewport[0]);

	/* open file and check */
	file = fopen(filename, "wb");
	
	if(!file)
	{
		fprintf(stderr, "Cannot open file for writing: %s\n", filename);
		return 1;
	}

	/* write headers */
	header[1] = viewport[2] / 256;
	header[0] = viewport[2] % 256;
	header[3] = viewport[3] / 256;
	header[2] = viewport[3] % 256;
	header[4] = 24;
	header[5] = 0;

	fwrite((char*)tga_header_uncompressed,
	       sizeof(tga_header_uncompressed), 1, file);

	fwrite((char*)header, sizeof(header), 1, file);

	/* read from GL */
	img_size = viewport[2] * viewport[3] * 3 * sizeof(char);
	buffer = (char*)malloc(img_size);
	glReadPixels(0, 0, viewport[2], viewport[3], GL_RGB, 
	             GL_UNSIGNED_BYTE, buffer);

	/* swap RB and write */
	for(i = 0; i < img_size; i += 3)
	{
		uint8_t temp = buffer[i];
		buffer[i] = buffer[i + 2];
		buffer[i + 2] = temp;
	}
	
	fwrite((char*)buffer, img_size, 1, file);

	/* end */
	fclose(file);
	free(buffer);
	return 0;
}

#if defined(GLF_GL_VERSION_GL3_XXX)

PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;

/** Set proc address */
#	define GLF_SPA(type, proc) \
                     proc = (type)glXGetProcAddressARB((const GLubyte*)#proc); \
                     if(proc == NULL) return 1;

/**
 * @brief Init the GL function pointers
 */
int glf_gl_init()
{
	GLF_SPA(PFNGLCREATESHADERPROC, glCreateShader)
	GLF_SPA(PFNGLCREATEPROGRAMPROC, glCreateProgram)
	GLF_SPA(PFNGLSHADERSOURCEPROC, glShaderSource)
	GLF_SPA(PFNGLCOMPILESHADERPROC, glCompileShader)
	GLF_SPA(PFNGLGETSHADERIVPROC, glGetShaderiv)
	GLF_SPA(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)
	GLF_SPA(PFNGLDELETESHADERPROC, glDeleteShader)
	GLF_SPA(PFNGLLINKPROGRAMPROC, glLinkProgram)
	GLF_SPA(PFNGLGETPROGRAMIVPROC, glGetProgramiv)
	GLF_SPA(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog)
	GLF_SPA(PFNGLATTACHSHADERPROC, glAttachShader)
	GLF_SPA(PFNGLDELETEPROGRAMPROC, glDeleteProgram)
	GLF_SPA(PFNGLUSEPROGRAMPROC, glUseProgram)
	GLF_SPA(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation)
	GLF_SPA(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation)
	GLF_SPA(PFNGLBINDBUFFERPROC, glBindBuffer)
	GLF_SPA(PFNGLGENBUFFERSPROC, glGenBuffers)
	GLF_SPA(PFNGLBUFFERDATAPROC, glBufferData)
	GLF_SPA(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv)
	GLF_SPA(PFNGLDELETEBUFFERSPROC, glDeleteBuffers)
	GLF_SPA(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays)
	GLF_SPA(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays)

	return 0;
}
#endif

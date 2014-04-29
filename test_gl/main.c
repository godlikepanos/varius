#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

SDL_Window *window;
SDL_GLContext ctx;
unsigned width = 800;
unsigned height = 800;

static void 
error(const char *err)
{
	fprintf(stderr, "%s\n", err);
}

static void 
check_gl_error(const char *file, int line, const char *func)
{
	GLenum err = glGetError();
	const char* glerr;

	if (err == GL_NO_ERROR)
		return;

	switch (err) {
	case GL_INVALID_ENUM:
		glerr = "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		glerr = "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		glerr = "GL_INVALID_OPERATION";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		glerr = "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;
	case GL_OUT_OF_MEMORY:
		glerr = "GL_OUT_OF_MEMORY";
		break;
	default:
		glerr = "unknown";
	};

	error(glerr);
}

#define CHECK_GL_ERROR() \
	check_gl_error(__FILE__, __LINE__, __func__)

static int 
set_sdl_attribs()
{
	int failed = SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) || 
		     SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8) || 
		     SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8) ||
		     SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8) ||
		     SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) ||
		     SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) ||
		     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4) ||
		     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4) ||
		     SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
			     		 SDL_GL_CONTEXT_PROFILE_CORE) ||
		     SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 
				     	 SDL_GL_CONTEXT_DEBUG_FLAG) ||
		     SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

	return failed;
}

static const GLenum gldbgsource[] = {
	GL_DEBUG_SOURCE_API,
	GL_DEBUG_SOURCE_WINDOW_SYSTEM,
	GL_DEBUG_SOURCE_SHADER_COMPILER,
	GL_DEBUG_SOURCE_THIRD_PARTY,
	GL_DEBUG_SOURCE_APPLICATION,
	GL_DEBUG_SOURCE_OTHER};

static const GLenum gldbgtype[] = {
	GL_DEBUG_TYPE_ERROR,
	GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
	GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
	GL_DEBUG_TYPE_PORTABILITY,
	GL_DEBUG_TYPE_PERFORMANCE,
	GL_DEBUG_TYPE_OTHER};

static const GLenum gldbgseverity[] = {
	GL_DEBUG_SEVERITY_LOW,
	GL_DEBUG_SEVERITY_MEDIUM,
	GL_DEBUG_SEVERITY_HIGH};

static void 
message_callback(GLenum source, GLenum type, GLuint id, 
		 GLenum severity, GLsizei length,
		 const char *message, GLvoid *user)
{
	(void)source;
	(void)id;
	(void)severity;
	(void)length;
	(void)user;
	printf("GL says: %s\n", message);
}

static int
register_messages()
{
	unsigned s, t, sv;

	glDebugMessageCallback(message_callback, NULL);

	for (s = 0; s < sizeof(gldbgsource) / sizeof(GLenum); s++)
		for (t = 0; t < sizeof(gldbgtype) / sizeof(GLenum); t++)
			for (sv = 0; 
			     sv < sizeof(gldbgseverity) / sizeof(GLenum); sv++)
				glDebugMessageControl(gldbgsource[s], 
						      gldbgtype[t], 
						      gldbgseverity[sv], 
						      0, 
						      NULL, 
						      GL_TRUE);

	CHECK_GL_ERROR();
	return 0;
}

static int
create_program(const char *source, const GLenum type, GLuint *program)
{
	GLuint prog;
	GLint status;

	prog = glCreateShaderProgramv(type, 1, &source);
	glGetProgramiv(prog, GL_LINK_STATUS, &status);

	if (status == GL_FALSE) {
		GLint len = 0;
		GLint written = 0;
		char *txt;

		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);

		txt = malloc(len + 1);
		glGetProgramInfoLog(prog, len, &written, txt);
		error(txt);
		free(txt);
		return 1;
	}

	*program = prog;
	return 0;
}

static int
create_ppline(const GLuint vert_prog, const GLuint frag_prog, GLuint *ppline)
{
	GLuint pp;
	GLint status;

	glGenProgramPipelines(1, &pp);
	glBindProgramPipeline(pp);

	glUseProgramStages(pp, GL_VERTEX_SHADER_BIT, vert_prog);
	glUseProgramStages(pp, GL_FRAGMENT_SHADER_BIT, frag_prog);

	glValidateProgramPipeline(pp);
	glGetProgramPipelineiv(pp, GL_VALIDATE_STATUS, &status);

	if (status == GL_FALSE) {
		error("Program ppline failed");
		return 1;
	}

	*ppline = pp;

	return 0;
}

static int
test_shader_storage_and_array_buffers()
{
	static const char *vert_source = 
		"#version 440 core\n"
		"layout(location = 0) in vec3 position;\n"
		"out gl_PerVertex {\n"
		"vec4 gl_Position;\n"
		"};\n"
		"void main()\n"
		"{\n"
		"\tgl_Position = vec4(position * 0.9, 1.0);\n"
		"}\n";

	static const char *frag_source = 
		"#version 440 core\n"
		"layout(location = 0) out vec3 out_color;\n"
		"void main()\n"
		"{\n"
		"\tout_color = vec3(0.0, 1.0, 1.0);\n"
		"}\n";

	GLuint vert_prog, frag_prog, ppline;
	GLuint buff, vao;
	static const GLbitfield map_flags = GL_MAP_WRITE_BIT | 
					    GL_MAP_PERSISTENT_BIT | 
					    GL_MAP_COHERENT_BIT;
	void *mapped;

	static const float verts[] = {
		1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, -1.0};

	if (create_program(vert_source, GL_VERTEX_SHADER, &vert_prog) || 
	    create_program(frag_source, GL_FRAGMENT_SHADER, &frag_prog)) {
		error("create_program() failed");
		return 1;
	}

	if (create_ppline(vert_prog, frag_prog, &ppline)) {
		error("create_ppline() failed");
		return 1;
	}

	glGenBuffers(1, &buff);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(verts), NULL, map_flags);

	mapped = glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(verts),
				  map_flags);

	memcpy(mapped, verts, sizeof(verts));

	/* Vertex array */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	/* Draw quad */
	glClearColor(1.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	return 0;
}

static int
test_shader_arrays()
{
	static const char *vert_source = 
		"#version 440 core\n"
		"layout(location = 0) in vec3 position;\n"
		"layout(std140) readonly buffer b_buff {\n"
		"\tmat4 u_mvp[8U];\n"
		"};\n"
		"out gl_PerVertex {\n"
		"vec4 gl_Position;\n"
		"};\n"
		"void main()\n"
		"{\n"
		"\tgl_Position = u_mvp[gl_InstanceID] * "
		"vec4(position * 0.9, 1.0);\n"
		"}\n";

	GLuint prog;
	GLint count;
#if 1
	static const GLenum interface = GL_BUFFER_VARIABLE;
#else
	static const GLenum interface = GL_UNIFORM;
#endif
	unsigned i;

	if (create_program(vert_source, GL_VERTEX_SHADER, &prog)) {
		error("create_program() failed");
		return 1;
	}

	glGetProgramInterfaceiv(prog, interface, GL_ACTIVE_RESOURCES, &count);

	for (i = 0; i < count; i++) {
		char name[256];
		GLsizei len;
		GLenum props[] = {GL_ARRAY_SIZE};
		GLenum out[1];

		glGetProgramResourceName(prog, interface, i, sizeof(name),
					 &len, name);

		name[len] = '\0';

		glGetProgramResourceiv(prog, interface, i, 
				       1, props,
				       1, NULL, out);

		printf("name: %s array_size: %d\n", name, out[0]);
	}

	return 0;
}

int 
main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS | 
	    SDL_INIT_GAMECONTROLLER)) {
		error("SDL_Init() failed");
		return 1;
	}

	if (set_sdl_attribs()) {
		error("SDL_GL_SetAttribute() failed");
		return 1;
	}

	window = SDL_CreateWindow("Title", 
				  SDL_WINDOWPOS_UNDEFINED, 
				  SDL_WINDOWPOS_UNDEFINED, 
				  width, 
				  height, 
				  SDL_WINDOW_OPENGL);

	if (window == NULL) {
		error("SDL_CreateWindow() failed");
		return 1;
	}

	ctx = SDL_GL_CreateContext(window);
	if (ctx == NULL) {
		error("SDL_GL_CreateContext() failed");
		return 1;
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		error("GLEW initialization failed");
		return 1;
	}
	glGetError();

	if (register_messages())
		return 1;

	/*if (test_shader_storage_and_array_buffers())
		return 1;
	
	SDL_GL_SwapWindow(window);
	sleep(2);*/

	if (test_shader_arrays())
		return 1;

	return 0;
}

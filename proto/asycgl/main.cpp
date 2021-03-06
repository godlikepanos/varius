#include <iostream>
#include <thread>
#include <stdexcept>
#include <SDL.h>
#include <GL/glew.h>
#include "anki/gl/Descriptors.h"

SDL_Window* window;
SDL_GLContext ctx, ctx2;
unsigned width = 800;
unsigned height = 800;
GLuint vert_prog;
GLuint frag_prog;
GLuint vertBuff;

const char* vert_src = R"(
#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

out gl_PerVertex
{
	vec4 gl_Position;
};

layout(binding = 1, std140) buffer uBuffer
{
	vec4 color;
} ub;

layout(location = 0) out vec3 vColor;
layout(location = 1) out vec2 vCoords;

void main()
{
	gl_Position = vec4(position * 0.9, 1.0);
	vColor = color + ub.color.rgb;
	vCoords = position.xy * 0.5 + 0.5;
}
)";


const char* frag_src = R"(
#version 430 core
#extension GL_NV_gpu_shader5 : enable

layout(binding = 10, std140) uniform uBuffer
{
	vec3 color;
	uvec2 handle;
};

layout(binding = 11) uniform sampler2D tex;

layout(location = 0) in vec3 vColor;
layout(location = 1) in vec2 vCoords;

layout(location = 0) out vec3 fColor;

void main()
{
	fColor = (vColor + color) * 0.001 + texture(tex, vCoords).rgb;
}
)";

void glConditionalThrowException(const char* file, int line, const char* func)
{
	GLenum errId = glGetError();
	if(errId == GL_NO_ERROR)
	{
		return;
	}

	const char* glerr;
	switch(errId)
	{
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

	std::cerr << file << ":" << line << " " << func << " " << glerr 
		<< std::endl;
}

#define CHECK_GL_ERROR() \
	glConditionalThrowException(__FILE__, __LINE__, __func__)

GLuint createProg(GLenum type, const char* src)
{
	GLuint prog = glCreateShaderProgramv(type, 1, &src);

	GLint status;
	glGetProgramiv(prog, GL_LINK_STATUS, &status);

	if(status == GL_FALSE)
	{
		GLint infoLen = 0;
		GLint charsWritten = 0;
		std::string infoLogTxt;

		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &infoLen);

		infoLogTxt.resize(infoLen + 1);
		glGetProgramInfoLog(prog, infoLen, &charsWritten, &infoLogTxt[0]);
		std::cerr << "Link error log follows: " << infoLogTxt << std::endl;

		prog = 0;
	}

	if(prog)
	{
		GLint count;
		glGetProgramInterfaceiv(prog, GL_UNIFORM, GL_ACTIVE_RESOURCES, &count);
		std::cout << "Uniform count " << count << std::endl;

		for(int i = 0; i < count; i++)
		{
			char name[256];
			GLint len;
			glGetProgramResourceName(prog, GL_UNIFORM, i, sizeof(name),
				&len, name);
			name[len] = '\0';

			GLenum prop[] = {GL_OFFSET};
			GLint out[1];
			glGetProgramResourceiv(prog, GL_UNIFORM, i, 1, prop, 
				sizeof(prop), NULL, out);

			std::cout << name 
				<< " offset:" << out[0]
				<< std::endl;
		}

		glGetProgramInterfaceiv(prog, GL_UNIFORM_BUFFER, 
			GL_ACTIVE_RESOURCES, &count);
		std::cout << "Uniform buffers " << count << std::endl;

		for(int i = 0; i < count; i++)
		{
			char name[256];
			GLint len;
			glGetProgramResourceName(prog, GL_UNIFORM_BLOCK, i, sizeof(name),
				&len, name);
			name[len] = '\0';

			GLenum prop[] = {GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE};
			GLint out[2];
			glGetProgramResourceiv(prog, GL_UNIFORM_BLOCK, i, 2, prop, 
				sizeof(prop), NULL, out);

			std::cout << name 
				<< " binding:" << out[0]
				<< " size:" << out[1]
				<< std::endl;
		}

		glGetProgramInterfaceiv(prog, GL_SHADER_STORAGE_BLOCK, 
			GL_ACTIVE_RESOURCES, &count);
		std::cout << "Storage buffers " << count << std::endl;

		for(int i = 0; i < count; i++)
		{
			char name[256];
			GLint len;
			glGetProgramResourceName(prog, GL_SHADER_STORAGE_BLOCK, 
				i, sizeof(name),
				&len, name);
			name[len] = '\0';

			GLenum prop[] = {GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE};
			GLint out[2];
			glGetProgramResourceiv(prog, GL_SHADER_STORAGE_BLOCK, i, 2, prop, 
				sizeof(prop), NULL, out);

			std::cout << name 
				<< " binding:" << out[0]
				<< " size:" << out[1]
				<< std::endl;
		}

		std::cout << std::endl;
	}

	return prog;
}

GLuint createVertexBuffer()
{
	static float vert[] = {
		-1, -1, 0,
		0, 0, 0,

		1, -1, 0,
		0, 0, 0,

		-1, 1, 0,
		0, 0, 0,

		1, -1, 0,
		0, 0, 0,

		1, 1, 0,
		0, 0, 0,

		-1, 1, 0,
		0, 0, 0};

	GLuint buff;

	glGenBuffers(1, &buff);

	glBindBuffer(GL_ARRAY_BUFFER, buff);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vert), &vert[0], GL_STATIC_DRAW);

	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT 
		| GL_MAP_PERSISTENT_BIT;

	glBufferStorage(GL_ARRAY_BUFFER, sizeof(vert), &vert[0], flags);

	unsigned char* map = (unsigned char*)glMapBufferRange(
			GL_ARRAY_BUFFER, 0, sizeof(vert), 
			flags);

	CHECK_GL_ERROR();

	return buff;
}

void createTexture(GLuint& tex, GLuint64& handle)
{
	const unsigned char data[] = {
		255, 0, 0,
		0, 255, 0,
		0, 0, 255,
		255, 0, 255};

	CHECK_GL_ERROR();
	glGenTextures(1, &tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, 
		data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	handle = glGetTextureHandleNV(tex);
	glMakeTextureHandleResidentNV(handle);
	CHECK_GL_ERROR();
}

GLuint createPipeline(GLuint vert_prog, GLuint frag_prog)
{
	CHECK_GL_ERROR();

	GLuint ppline;
	glGenProgramPipelines(1, &ppline);
	glBindProgramPipeline(ppline);

	glUseProgramStages(ppline, GL_VERTEX_SHADER_BIT, vert_prog);
	glUseProgramStages(ppline, GL_FRAGMENT_SHADER_BIT, frag_prog);

	glValidateProgramPipeline(ppline);
	CHECK_GL_ERROR();

	GLint status = 1;
	glGetProgramPipelineiv(ppline, GL_VALIDATE_STATUS, &status);

	if(!status)
	{
		GLint infoLen = 0;
		GLint charsWritten = 0;
		std::string infoLogTxt;

		glGetProgramPipelineiv(ppline, GL_INFO_LOG_LENGTH, &infoLen);

		infoLogTxt.resize(infoLen + 1);
		glGetProgramPipelineInfoLog(
			ppline, infoLen, &charsWritten, &infoLogTxt[0]);
		std::cerr << "Ppline error log follows: " << infoLogTxt << std::endl;
	}

	return ppline;
}

GLuint createVao(GLuint buff)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buff);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 
		(const void*)(3 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vao;
}

void threadFunc()
{
	SDL_GL_MakeCurrent(window, ctx2);

	GLuint tex;
	GLuint64 texHandle;

	createTexture(tex, texHandle);
	glActiveTexture(GL_TEXTURE0 + 11);
	glBindTexture(GL_TEXTURE_2D, tex);

	vert_prog = createProg(GL_VERTEX_SHADER, vert_src);
	frag_prog = createProg(GL_FRAGMENT_SHADER, frag_src);
	vertBuff = createVertexBuffer();

	{
		GLint unit;
		glGetUniformiv(
			frag_prog, glGetUniformLocation(frag_prog, "tex"), &unit);

		std::cout << "[[[[[[[[[[[[[[" << unit << std::endl;
	}

	glDisable(GL_DEPTH_TEST);
	glViewport(0, 0, 800, 800);

	GLuint ppline = createPipeline(vert_prog, frag_prog);
	GLuint vao = createVao(vertBuff);

	glUseProgram(0);
	glBindProgramPipeline(ppline);

	// Create uniform buffer
	GLuint ubuff;
	glGenBuffers(1, &ubuff);
	glBindBuffer(GL_UNIFORM_BUFFER, ubuff);
	struct Foo
	{
		float col[4] = {0, 0, 0, 1};
		GLuint64 handle;
	} foo;
	foo.handle = texHandle;

	glBufferData(GL_UNIFORM_BUFFER, sizeof(Foo), &foo, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, 10, ubuff);
	CHECK_GL_ERROR();


	GLuint ubuff2;
	glGenBuffers(1, &ubuff2);
	glBindBuffer(GL_UNIFORM_BUFFER, ubuff2);
	float col[4] = {0, 0, 0, 1};
	glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 4, col, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ubuff2);
	CHECK_GL_ERROR();

	int iterations = 20;
	do
	{
		glClearColor((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 
			(float)rand() / RAND_MAX, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		/*col[0] = (float)rand() / RAND_MAX;
		col[1] = (float)rand() / RAND_MAX;
		col[2] = (float)rand() / RAND_MAX;*/
		col[0] = 0.5;
		col[1] = 0;
		col[2] = 0;

		glBindBuffer(GL_UNIFORM_BUFFER, ubuff);	
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 4, col, GL_DYNAMIC_DRAW);

		col[0] = 0;
		col[1] = 0.5;
		col[2] = 0;

		glBindBuffer(GL_UNIFORM_BUFFER, ubuff2);	
		glBufferData(GL_UNIFORM_BUFFER, sizeof(float) * 4, col, GL_DYNAMIC_DRAW);


		CHECK_GL_ERROR();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		SDL_GL_SwapWindow(window);

		SDL_Delay(100);

	} while(iterations--);

	CHECK_GL_ERROR();
	
	glFinish();
	SDL_GL_MakeCurrent(window, nullptr);
}

int main(int, char**)
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS 
		| SDL_INIT_GAMECONTROLLER) != 0)
	{
		throw std::runtime_error("SDL_Init() failed");
	}

	if(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8) != 0
		|| SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) != 0
		|| SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0
		|| SDL_GL_SetAttribute(
			SDL_GL_CONTEXT_MAJOR_VERSION, 4) != 0
		|| SDL_GL_SetAttribute(
			SDL_GL_CONTEXT_MINOR_VERSION, 3) != 0
		|| SDL_GL_SetAttribute(
			SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0
		|| SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1))
	{
		throw std::runtime_error("SDL_GL_SetAttribute() failed");
	}

	window = SDL_CreateWindow(
    	"Title", 
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
		width, height, SDL_WINDOW_OPENGL);

	if(window == nullptr)
	{
		throw std::runtime_error("SDL_CreateWindow() failed");
	}

	ctx2 = SDL_GL_CreateContext(window);
	if(ctx2 == nullptr)
	{
		throw std::runtime_error("SDL_GL_CreateContext() failed");
	}

	ctx = SDL_GL_CreateContext(window);
	if(ctx == nullptr)
	{
		throw std::runtime_error("SDL_GL_CreateContext() failed");
	}

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		throw std::runtime_error("GLEW initialization failed");
	}
	glGetError();


	std::thread t1(threadFunc);
	t1.join();
	//threadFunc();

	return 0;
}

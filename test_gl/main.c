#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <stdio.h>

SDL_Window *window;
SDL_GLContext ctx;
unsigned width = 800;
unsigned height = 800;

void error(const char *err)
{
	fprintf(stderr, "%s\n", err);
	exit(1);
}

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_EVENTS 
		| SDL_INIT_GAMECONTROLLER) != 0) {
		error("SDL_Init() failed");
	}

	if (SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8) != 0
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
		|| SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1)) {
		error("SDL_GL_SetAttribute() failed");
	}

	window = SDL_CreateWindow("Title", 
							  SDL_WINDOWPOS_UNDEFINED, 
							  SDL_WINDOWPOS_UNDEFINED, 
							  width, 
							  height, 
							  SDL_WINDOW_OPENGL);

	if (window == NULL) {
		error("SDL_CreateWindow() failed");
	}

	ctx = SDL_GL_CreateContext(window);
	if (ctx == NULL) {
		error("SDL_GL_CreateContext() failed");
	}

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		error("GLEW initialization failed");
	}
	glGetError();

	return 0;
}

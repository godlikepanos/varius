#include <cstdlib>
#include <cmath>
#include <GL/gl.h>
#include <GL/glut.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include "Application.h"


//======================================================================================================================
// Constructor                                                                                                         =
//======================================================================================================================
Application::Application(int width, int height):
	physics(NULL),
	camPos(0.0, 0.0, 5.0),
	camZ(0.0, 0.0, 1.0),
	camY(0.0, 1.0, 0.0),
	movingDist(0.2),
	movingAng(0.1),
	keys(SDLK_LAST, 0),
	time(0)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 8);
	mainSurf = SDL_SetVideoMode(width, height, 24, SDL_HWSURFACE | SDL_OPENGL);

	// move window
	/*SDL_SysWMinfo myinfo;
	Display *d;
	SDL_GetWMInfo(&myinfo);
	if(myinfo.subsystem == SDL_SYSWM_X11)
	{
		d = myinfo.info.x11.display;
		myinfo.info.x11.lock_func();
		XMoveWindow(d, myinfo.info.x11.wmwindow, 400, 800);
		myinfo.info.x11.unlock_func();
	}*/


	initGl();
	resize(width, height);
	
	// init physics
	physics = new Physics;
}


//======================================================================================================================
// exec                                                                                                                =
//======================================================================================================================
int Application::exec()
{
	while(1)
	{
		inputCallback();
		processInput();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(camPos.getX(), camPos.getY(), camPos.getZ(), camPos.getX() - camZ.getX(), camPos.getY() - camZ.getY(),
							camPos.getZ() - camZ.getZ(), camY.getX(), camY.getY(), camY.getZ());

		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

		renderGrid();

		mainLoop();

		SDL_GL_SwapBuffers();
		waitForNextFrame();
	}
	
	return 0;
}


//======================================================================================================================
// reorthogonalize                                                                                                     =
//======================================================================================================================
void Application::reorthogonalize()
{
	btVector3 camX;
	camX = camY.cross(camZ);
	camY = camZ.cross(camX);
}


//======================================================================================================================
// renderGrid                                                                                                          =
//======================================================================================================================
void Application::renderGrid()
{
	float col0[] = {0.5, 0.5, 0.5};
	float col1[] = {0.0, 0.0, 1.0};
	float col2[] = {1.0, 0.0, 0.0};

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_LINE_STIPPLE);
	glColor3fv(col0);

	const float space = 1.0; // space between lines
	const int num = 57; // lines number. must be odd

	float opt = ((num - 1) * space / 2);
	glBegin(GL_LINES);
	for(int x = 0; x < num; x++)
	{
		if(x == num / 2) // if the middle line then chmovingAnge color
		glColor3fv(col1);
		else if(x == (num / 2) + 1) // if the next line after the middle one chmovingAnge back to default col
		glColor3fv(col0);

		float opt1 = (x * space);
		// line in z
		glVertex3f(opt1 - opt, 0.0, -opt);
		glVertex3f(opt1 - opt, 0.0, opt);

		if(x == num / 2) // if middle line chmovingAnge col so you can highlight the x-axis
		glColor3fv(col2);

		// line in the x
		glVertex3f(-opt, 0.0, opt1 - opt);
		glVertex3f(opt, 0.0, opt1 - opt);
	}
	glEnd();
	
	glLineWidth(4.0);
	glBegin(GL_LINES);
	glColor3fv(col2);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	glColor3fv(col1);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 100.0);
	glEnd();
	glLineWidth(1.0);
}


//======================================================================================================================
// processInput                                                                                                        =
//======================================================================================================================
void Application::processInput()
{
	btVector3 camX;
	camX = camY.cross(camZ);

	if(keys[SDLK_ESCAPE])
	{
		exit(0);
	}

	if(keys[SDLK_w])
	{
		camPos += camZ * -movingDist;
	}

	if(keys[SDLK_a])
	{
		camPos += camX * -movingDist;
	}

	if(keys[SDLK_s])
	{
		camPos += camZ * movingDist;
	}

	if(keys[SDLK_d])
	{
		camPos += camX * movingDist;
	}

	if(keys[SDLK_SPACE])
	{
		camPos += camY * -movingDist;
	}

	if(keys[SDLK_LSHIFT])
	{
		camPos += camY * movingDist;
	}

	if(keys[SDLK_q])
	{
		float s = sin(-movingAng);
		float c = cos(-movingAng);

		camY = camY * c + camX * s;

		camY.normalize();
	}

	if(keys[SDLK_e])
	{
		float s = sin(movingAng);
		float c = cos(movingAng);

		camY = camY * c + camX * s;

		camY.normalize();
	}

	if(keys[SDLK_LEFT])
	{
		float s = sin(movingAng);
		float c = cos(movingAng);

		camZ = camZ * c + camX * s;

		camZ.normalize();
	}

	if(keys[SDLK_RIGHT])
	{
		float s = sin(-movingAng);
		float c = cos(-movingAng);

		camZ = camZ * c + camX * s;

		camZ.normalize();
	}

	if(keys[SDLK_UP])
	{
		float s = sin(-movingAng);
		float c = cos(-movingAng);

		camZ = camZ * c + camY * s;

		camZ.normalize();
	}

	if(keys[SDLK_DOWN])
	{
		float s = sin(movingAng);
		float c = cos(movingAng);

		camZ = camZ * c + camY * s;

		camZ.normalize();

		camY = camZ.cross(camX);
	}

	reorthogonalize();
}


//======================================================================================================================
// resize                                                                                                              =
//======================================================================================================================
void Application::resize(int w, int h)
{
	if(h == 0) h = 1;

	float ratio = 1.0 * w / h;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45, ratio, 1, 1000);
}


//======================================================================================================================
// initGl                                                                                                              =
//======================================================================================================================
void Application::initGl()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_MULTISAMPLE);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}


//======================================================================================================================
// inputCallback                                                                                                       =
//======================================================================================================================
void Application::inputCallback()
{
	// add the times a key is being pressed
	for(uint x = 0; x < keys.size(); x++)
	{
		if(keys[x]) ++keys[x];
	}

	SDL_Event event_;
	while(SDL_PollEvent(&event_))
	{
		switch(event_.type)
		{
			case SDL_KEYDOWN:
				keys[event_.key.keysym.sym] = 1;
				break;

			case SDL_KEYUP:
				keys[event_.key.keysym.sym] = 0;
				break;

			case SDL_QUIT:
				exit(0);
				break;

			case SDL_VIDEORESIZE:
				resize(event_.resize.w, event_.resize.h);
				break;
		}
	}
}


//======================================================================================================================
// waitForNextFrame                                                                                                    =
//======================================================================================================================
void Application::waitForNextFrame()
{
	const uint timerTick = 1000/40;

	uint now = SDL_GetTicks();

	if(now - time < timerTick)
	{
		// the new time after the SDL_Delay will be...
		time += timerTick;
		// sleep a little
		SDL_Delay(time - now);
	}
	else time = now;
}

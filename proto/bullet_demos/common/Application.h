#ifndef APP_H
#define APP_H

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include <Physics.h>


using namespace std;


/**
 * The demo application. Creates the window, inits OpenGL and runs the main loop
 */
class Application
{
	public:
		Physics* physics;
		vector<int> keys;
	
		Application(int width, int height);
		int exec();

	private:
		class SDL_Surface* mainSurf;
		btVector3 camPos;
		btVector3 camZ;
		btVector3 camY;
		float movingDist;
		float movingAng;
		uint time;

		void reorthogonalize();

		void renderGrid();

		void processInput();

		void resize(int w, int h);

		void initGl();

		void inputCallback();

		virtual void mainLoop() = 0;

		void waitForNextFrame();
};

#endif

#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include "Application.h"


/**
 *
 */
class MyApplication: public Application
{
	public:
		class Character* character;

		MyApplication(int width, int height):
			Application(width, height)
		{}

		void initAdditionalPhysics();

	private:
		btClock clock;

		void mainLoop();
};


#endif

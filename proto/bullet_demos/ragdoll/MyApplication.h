#ifndef PARTICLEAPPLICATION_H
#define PARTICLEAPPLICATION_H

#include "Application.h"
#include "Ragdoll.h"


/**
 * My application
 */
class MyApplication: public Application
{
	public:
		MyApplication(int width, int height):
			Application(width, height)
		{}
		
		void initAdditionalPhysics();

	private:
		btClock clock;
		Ragdoll* ragdoll;
		Ragdoll::Initializer init;

		void mainLoop();
};


#endif


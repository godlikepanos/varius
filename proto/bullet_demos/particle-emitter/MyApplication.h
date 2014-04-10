#ifndef PARTICLEAPPLICATION_H
#define PARTICLEAPPLICATION_H

#include "Application.h"


class ParticleEmitter;


/**
 * @todo
 */
class MyApplication: public Application
{
	public:
		ParticleEmitter* pe;
	
		MyApplication(int width, int height):
			Application(width, height)
		{}
		
		void initAdditionalPhysics();

	private:
		btClock clock;

		void mainLoop();
};


#endif


#include <SDL/SDL.h>
#include "MyApplication.h"
#include "Character.h"


//======================================================================================================================
// mainLoop                                                                                                            =
//======================================================================================================================
void MyApplication::mainLoop()
{
	clock.reset();

	float ang = 0.01;
	float dist = 0.1;

	if(keys[SDLK_i])
		character->moveForward(dist);

	if(keys[SDLK_k])
		character->moveForward(-dist);

	if(keys[SDLK_j])
		character->rotate(ang);

	if(keys[SDLK_l])
		character->rotate(-ang);

	if(keys[SDLK_p])
		character->jump();

	physics->mainLoop();

	character->moveForward(0.0);
	//cout << "Drawing time: " << clock.getTimeMicroseconds() << "μs" << endl;
}


//======================================================================================================================
// initAdditionalPhysics                                                                                               =
//======================================================================================================================
void MyApplication::initAdditionalPhysics()
{
	// ground
	btCollisionShape* groundShape = new btBoxShape(btVector3(50.0, 50.0, 50.0));
	btTransform trf;
	trf.setIdentity();
	trf.setOrigin(btVector3(0, -50, 0));
	physics->createNewRigidBody(0.0, trf, groundShape/*, Physics::CG_MAP, Physics::CG_ALL*/);


	btCollisionShape* shape = new btBoxShape(btVector3(5.0, 5.0, 5.0));

	trf.setOrigin(btVector3(0, 0, -10));
	trf.setRotation(btQuaternion(btVector3(1, 0, 0), 1.9));
	physics->createNewRigidBody(0.0, trf, shape/*, Physics::CG_MAP, Physics::CG_ALL*/);

	trf.setIdentity();
	trf.setOrigin(btVector3(0, -2.9, 0));
	physics->createNewRigidBody(0.0, trf, shape/*, Physics::CG_MAP, Physics::CG_ALL*/);

	trf.setIdentity();
	trf.setOrigin(btVector3(0, -4.2, 2));
	physics->createNewRigidBody(0.0, trf, shape/*, Physics::CG_MAP, Physics::CG_ALL*/);

	// char
	Character::Initializer init;
	character = new Character(*physics, init);
}

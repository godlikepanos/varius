#include "MyApplication.h"
#include "ParticleEmitter.h"


//======================================================================================================================
// mainLoop                                                                                                            =
//======================================================================================================================
void MyApplication::mainLoop()
{
	pe->update();
	clock.reset();
	physics->mainLoop();
	//cout << "Drawing time: " << clock.getTimeMicroseconds() << "μs" << endl;
}


//======================================================================================================================
// initAdditionalPhysics                                                                                               =
//======================================================================================================================
void MyApplication::initAdditionalPhysics()
{
	// ground
	btCollisionShape* groundShape = new btBoxShape(btVector3(50.0, 50.0, 50.0));
	btTransform groundTransform;
	groundTransform.setIdentity();

	groundTransform.setOrigin(btVector3(0, -50, 0));
	physics->createNewRigidBody(0.0, groundTransform, groundShape, Physics::CG_MAP, Physics::CG_ALL);

	groundTransform.setOrigin(btVector3(60, 0, 0));
	physics->createNewRigidBody(0.0, groundTransform, groundShape, Physics::CG_MAP, Physics::CG_ALL);

	groundTransform.setOrigin(btVector3(-60, 0, 0));
	physics->createNewRigidBody(0.0, groundTransform, groundShape, Physics::CG_MAP, Physics::CG_ALL);

	groundTransform.setOrigin(btVector3(0, 0, -60));
	physics->createNewRigidBody(0.0, groundTransform, groundShape, Physics::CG_MAP, Physics::CG_ALL);

	groundTransform.setOrigin(btVector3(0, 0, 60));
	physics->createNewRigidBody(0.0, groundTransform, groundShape, Physics::CG_MAP, Physics::CG_ALL);

	groundTransform.setOrigin(btVector3(0, 80, 0));
	physics->createNewRigidBody(0.0, groundTransform, groundShape, Physics::CG_MAP, Physics::CG_ALL);
	
	pe = new ParticleEmitter;
	pe->init();
}

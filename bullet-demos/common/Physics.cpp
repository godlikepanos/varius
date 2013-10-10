#include "Physics.h"
#include "DebugDraw.h"



//======================================================================================================================
// createNewRigidBody                                                                                                  =
//======================================================================================================================
btRigidBody* Physics::createNewRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape,
                                         short group, short mask)
{
	bool isDynamic = (mass != 0.0);

	btVector3 localInertia(0, 0, 0);
	if(isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);

	btRigidBody* body = new btRigidBody(cInfo);

	if(mask == -1 || group == -1)
		dynamicsWorld->addRigidBody(body);
	else
		dynamicsWorld->addRigidBody(body, group, mask);
		
	body->setActivationState(ISLAND_SLEEPING);

	return body;
}
		

//======================================================================================================================
// mainLoop                                                                                                            =
//======================================================================================================================
void Physics::mainLoop()
{
	float ms = myClock.getTimeMicroseconds();
	myClock.reset();
	dynamicsWorld->stepSimulation(ms / 1000000.0);
	dynamicsWorld->debugDrawWorld();
}

		
//======================================================================================================================
// init                                                                                                                =
//======================================================================================================================
void Physics::init()
{
	collCfg = new btDefaultCollisionConfiguration();

	// threading
	/*{
		int maxNumOutstandingTasks = 4;

		PosixThreadSupport::ThreadConstructionInfo constructionInfo("collision", processCollisionTask,
																																createCollisionLocalStoreMemory,
																																maxNumOutstandingTasks);
		threadSupportCollision = new PosixThreadSupport(constructionInfo);
	}*/

	dispatcher = new btCollisionDispatcher(collCfg);
	broadphase = new btAxisSweep3(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000));
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collCfg);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	debugDraw = new DebugDraw();
	dynamicsWorld->setDebugDrawer(debugDraw);
	dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
}

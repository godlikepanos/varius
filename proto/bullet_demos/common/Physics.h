#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>


using namespace std;


class DebugDraw;


/**
 * Control class for physics
 */
class Physics
{
	public:
		enum CollisionGroup
		{
			CG_NOTHING = 0,
			CG_MAP = 1,
			CG_PARTICLE = 2,
			CG_ALL = CG_MAP | CG_PARTICLE
		};

	public:
		btDefaultCollisionConfiguration* collCfg;
		btCollisionDispatcher* dispatcher;
		btBroadphaseInterface* broadphase;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		btClock myClock;
		DebugDraw* debugDraw;

		// threading
		//btThreadSupportInterface*	threadSupportCollision;

		/**
		 * Constructor
		 */
		Physics();

		/**
		 * Create a new rigid body and register it
		 */
		btRigidBody* createNewRigidBody(float mass, const btTransform& startTransform, btCollisionShape* shape,
		                                short group = -1, short mask = -1);

		/**
		 * physics main loop callback
		 */
		void mainLoop();

	private:
		/**
		 * Init physics
		 */
		void init();
};


inline Physics::Physics()
{
	init();
}


inline ostream& operator<<(ostream& s, const btVector3& v)
{
	s << v.getX() << " " << v.getY() << " " << v.getZ();
	return s;
}


#endif

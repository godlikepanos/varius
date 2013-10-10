#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>


using namespace std;

class Physics;


/**
 * @todo
 */
class Character
{
	public:
		struct Initializer
		{
			btScalar characterHeight;
			btScalar characterWidth;
			btScalar stepHeight;
			btScalar jumpSpeed;
			btScalar maxJumpHeight;

			Initializer();
		};

		Character(Physics& physics, const Initializer& init);
		void rotate(float angle);
		void moveForward(float distance);
		void jump();

	private:
		btPairCachingGhostObject* ghostObject;
		btConvexShape* convexShape;
		btKinematicCharacterController* character;
};


inline Character::Initializer::Initializer():
	characterHeight(2.0),
	characterWidth(0.75),
	stepHeight(1.0),
	jumpSpeed(10.0),
	maxJumpHeight(.0)
{}


#endif

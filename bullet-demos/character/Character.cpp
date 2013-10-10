#include "Character.h"
#include "Physics.h"


//======================================================================================================================
// Constructor                                                                                                         =
//======================================================================================================================
Character::Character(Physics& physics, const Initializer& init)
{
	ghostObject = new btPairCachingGhostObject();

	btAxisSweep3* sweepBp = dynamic_cast<btAxisSweep3*>(physics.broadphase);
	if(sweepBp == NULL)
	{
		cerr << "Error: See file" << endl;
		return;
	}

	sweepBp->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	//collisionShape = new btCapsuleShape(init.characterWidth, init.characterHeight);
	convexShape = new btCylinderShape(btVector3(init.characterWidth, init.characterHeight, init.characterWidth));

	ghostObject->setCollisionShape(convexShape);
	//ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
	btTransform trf;
	trf.setIdentity();
	trf.setOrigin(btVector3(0.0, 40.0, 0.0));
	ghostObject->setWorldTransform(trf);

	character = new btKinematicCharacterController(ghostObject, convexShape, init.stepHeight);

	character->setJumpSpeed(init.jumpSpeed);
	character->setMaxJumpHeight(init.maxJumpHeight);

	// register
	physics.dynamicsWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter,
																						btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);

	physics.dynamicsWorld->addAction(character);
}


//======================================================================================================================
// rotate                                                                                                              =
//======================================================================================================================
void Character::rotate(float angle)
{
	btMatrix3x3 rot = ghostObject->getWorldTransform().getBasis();
	rot *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), angle));
	ghostObject->getWorldTransform().setBasis(rot);
}


//======================================================================================================================
// moveForward                                                                                                         =
//======================================================================================================================
void Character::moveForward(float distance)
{
	btVector3 forward = -ghostObject->getWorldTransform().getBasis().getColumn(2);
	character->setWalkDirection(forward * distance);
}


//======================================================================================================================
// jump                                                                                                                =
//======================================================================================================================
void Character::jump()
{
	character->jump();
}

#include <algorithm>
#include "Ragdoll.h"
#include "Physics.h"


const float PI = 3.14159265358979323846;
const float CONSTRAINT_DEBUG_SIZE = 0.2;


//======================================================================================================================
// Constructor                                                                                                         =
//======================================================================================================================
Ragdoll::Ragdoll(Physics& physics_, const Initializer& init):
	physics(physics_)
{
	/*
	 * Create shapes

	shapes[BP_HEAD] = new btSphereShape(0.1);
	shapes[BP_SPINE_UP] = new btCapsuleShapeX(0.12, 0.28);
	shapes[BP_SPINE_DOWN] = shapes[BP_SPINE_UP];
	shapes[BP_PELVIS] = new btCapsuleShape(0.05, 0.1);
	shapes[BP_ARM_L_UP] = new btCapsuleShape(0.05, 0.33);
	shapes[BP_ARM_L_DOWN] = new btCapsuleShape(0.04, 0.39);
	shapes[BP_HAND_L] = new btCapsuleShape(0.05, 0.01);;
	shapes[BP_ARM_R_UP] = shapes[BP_ARM_L_UP];
	shapes[BP_ARM_R_DOWN] = shapes[BP_ARM_L_DOWN];
	shapes[BP_HAND_R] = shapes[BP_HAND_L];
	shapes[BP_LEG_L_UP] = new btCapsuleShape(0.07, 0.45);
	shapes[BP_LEG_L_DOWN] = new btCapsuleShape(0.05, 0.37);
	shapes[BP_FOOT_L] = new btCapsuleShape(0.02, 0.07);
	shapes[BP_LEG_R_UP] = shapes[BP_LEG_L_UP];
	shapes[BP_LEG_R_DOWN] = shapes[BP_LEG_L_DOWN];
	shapes[BP_FOOT_R] = shapes[BP_FOOT_L];



	 * create bodies

	float mass = 0.1;
	short group = -1, mask = -1;
	btTransform trf;
	trf.setIdentity();

	trf.setOrigin(btVector3(0.0, 1.0, 0.0));
	bodies[BP_HEAD] = physics.createNewRigidBody(mass, trf, shapes[BP_HEAD], group, mask);
	trf.getOrigin().setY(trf.getOrigin().getY() - 0.3);
	bodies[BP_SPINE_UP] = physics.createNewRigidBody(mass, trf, shapes[BP_SPINE_UP], group, mask);
	trf.getOrigin().setY(trf.getOrigin().getY() - 0.28);
	bodies[BP_SPINE_DOWN] = physics.createNewRigidBody(mass, trf, shapes[BP_SPINE_DOWN], group, mask);
	trf.getOrigin().setY(trf.getOrigin().getY() - 0.2);
	bodies[BP_PELVIS] = physics.createNewRigidBody(mass, trf, shapes[BP_PELVIS], group, mask);
	bodies[BP_ARM_L_UP] = ;
	bodies[BP_ARM_L_DOWN] = ;
	bodies[BP_HAND_L] = ;
	bodies[BP_ARM_R_UP] = ;
	bodies[BP_ARM_R_DOWN] = ;
	bodies[BP_HAND_R] = ;
	bodies[BP_LEG_L_UP] = ;
	bodies[BP_LEG_L_DOWN] = ;
	bodies[BP_FOOT_L] = ;
	bodies[BP_LEG_R_UP] = ;
	bodies[BP_LEG_R_DOWN] = ;
	bodies[BP_FOOT_R] = ;



	 * Connect

	btTransform localA, localB;
	btHingeConstraint* hingeC;
	btConeTwistConstraint* coneC;

	localA.setIdentity();
	localB.setIdentity();
	localA.getBasis().setEulerZYX(0, 0, PI/2);
	localA.setOrigin(btVector3(0.0, 0.30, 0.0));
	localB.getBasis().setEulerZYX(0, 0, PI/2);
	localB.setOrigin(btVector3(0., -0.14, 0.));
	coneC = new btConeTwistConstraint(*bodies[BP_SPINE_UP], *bodies[BP_HEAD], localA, localB);
	coneC->setLimit(PI/4, PI/4, PI/2);
	joints[J_NECK] = coneC;
	coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);

	physics.dynamicsWorld->addConstraint(joints[J_NECK], true);*/

	/*
	 * Create shapes
	 */
	for(uint i=0; i<BP_NUM; i++)
	{
		float width = 0.1;
		const btVector3 head = init.heads[i];
		const btVector3 tail = init.tails[i];
		float length = (head - tail).length();

		if(i == BP_SPINE_UP)
		{
			btVector3 max(fabs(init.heads[BP_ARM_L_UP].getX()), length/2, 0.2);
			shapes[i] = new btBoxShape(max);
		}
		else
			shapes[i] = new btCapsuleShape(width, length - width * 2);
	}

	/*
	 * Create bodies
	 */
	for(uint i=0; i<BP_NUM; i++)
	{
		float mass = 0.1;
		short group = -1, mask = -1;

		// set the transformation
		btTransform trf;
		trf.setIdentity();
		const btVector3& head = init.heads[i];
		const btVector3& tail = init.tails[i];
		trf.setOrigin((head + tail) / 2.0);
		btVector3 bi, bj, bk; // basis vectors i, j, k
		bj = tail - head;
		bj.normalize();
		bi = btVector3(1, 0, 0);
		bk = bi.cross(bj);
		bk.normalize();
		bi = bj.cross(bk);
		btMatrix3x3 rot;
		rot[0][0] = bi.x();
		rot[0][1] = bj.x();
		rot[0][2] = bk.x();
		rot[1][0] = bi.y();
		rot[1][1] = bj.y();
		rot[1][2] = bk.y();
		rot[2][0] = bi.z();
		rot[2][1] = bj.z();
		rot[2][2] = bk.z();
		trf.setBasis(rot);

		// create body
		bodies[i] = physics.createNewRigidBody(mass, trf, shapes[i], group, mask);

		// Setup some damping on the m_bodies
		/*bodies[i]->setDamping(0.05, 0.85);
		bodies[i]->setDeactivationTime(0.8);
		bodies[i]->setSleepingThresholds(1.6, 2.5);*/
	}

	/*
	 * Constraints
	 */
	btPoint2PointConstraint* c;
	for(uint i=BP_HEAD; i<=BP_PELVIS-1; i++)
	{
		c = new btPoint2PointConstraint(*bodies[i], *bodies[i+1],
		                                bodies[i]->getWorldTransform().inverse() * init.heads[i],
		                                bodies[i+1]->getWorldTransform().inverse() * init.tails[i+1]);
		physics.dynamicsWorld->addConstraint(c, true);
	}

	for(uint i=BP_ARM_L_UP; i<=BP_HAND_L-1; i++)
	{
		c = new btPoint2PointConstraint(*bodies[i], *bodies[i+1],
		                                bodies[i]->getWorldTransform().inverse() * init.tails[i],
		                                bodies[i+1]->getWorldTransform().inverse() * init.heads[i+1]);
		physics.dynamicsWorld->addConstraint(c, true);
	}

	for(uint i=BP_LEG_L_UP; i<=BP_FOOT_L-1; i++)
	{
		c = new btPoint2PointConstraint(*bodies[i], *bodies[i+1],
		                                bodies[i]->getWorldTransform().inverse() * init.tails[i],
		                                bodies[i+1]->getWorldTransform().inverse() * init.heads[i+1]);
		physics.dynamicsWorld->addConstraint(c, true);
	}

	for(uint i=BP_ARM_R_UP; i<=BP_HAND_R-1; i++)
	{
		c = new btPoint2PointConstraint(*bodies[i], *bodies[i+1],
		                                bodies[i]->getWorldTransform().inverse() * init.tails[i],
		                                bodies[i+1]->getWorldTransform().inverse() * init.heads[i+1]);
		physics.dynamicsWorld->addConstraint(c, true);
	}

	for(uint i=BP_LEG_R_UP; i<=BP_FOOT_R-1; i++)
	{
		c = new btPoint2PointConstraint(*bodies[i], *bodies[i+1],
		                                bodies[i]->getWorldTransform().inverse() * init.tails[i],
		                                bodies[i+1]->getWorldTransform().inverse() * init.heads[i+1]);
		physics.dynamicsWorld->addConstraint(c, true);
	}

	c = new btPoint2PointConstraint(*bodies[BP_SPINE_UP], *bodies[BP_ARM_L_UP],
	                                bodies[BP_SPINE_UP]->getWorldTransform().inverse() * init.tails[BP_SPINE_UP] + btVector3(-1, 0, 0),
	                                bodies[BP_ARM_L_UP]->getWorldTransform().inverse() * init.heads[BP_ARM_L_UP]);
	physics.dynamicsWorld->addConstraint(c, true);

	c = new btPoint2PointConstraint(*bodies[BP_SPINE_UP], *bodies[BP_ARM_R_UP],
	                                bodies[BP_SPINE_UP]->getWorldTransform().inverse() * init.tails[BP_SPINE_UP] + btVector3(1, 0, 0),
	                                bodies[BP_ARM_R_UP]->getWorldTransform().inverse() * init.heads[BP_ARM_R_UP]);
	physics.dynamicsWorld->addConstraint(c, true);


	c = new btPoint2PointConstraint(*bodies[BP_PELVIS], *bodies[BP_LEG_L_UP],
	                                bodies[BP_PELVIS]->getWorldTransform().inverse() * init.heads[BP_PELVIS],
	                                bodies[BP_LEG_L_UP]->getWorldTransform().inverse() * init.heads[BP_LEG_L_UP]);
	physics.dynamicsWorld->addConstraint(c, true);

	c = new btPoint2PointConstraint(*bodies[BP_PELVIS], *bodies[BP_LEG_R_UP],
	                                bodies[BP_PELVIS]->getWorldTransform().inverse() * init.heads[BP_PELVIS],
	                                bodies[BP_LEG_R_UP]->getWorldTransform().inverse() * init.heads[BP_LEG_R_UP]);
	physics.dynamicsWorld->addConstraint(c, true);
}


//======================================================================================================================
// activate                                                                                                            =
//======================================================================================================================
void Ragdoll::activate()
{
	for(uint i=0; i<BP_NUM; i++)
	{
		bodies[i]->setActivationState(ACTIVE_TAG);
	}
}

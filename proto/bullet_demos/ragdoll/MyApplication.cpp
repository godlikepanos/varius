#include <GL/gl.h>
#include <SDL/SDL.h>
#include "MyApplication.h"
#include "Ragdoll.h"


const float PI = 3.14159265358979323846;


//======================================================================================================================
// mainLoop                                                                                                            =
//======================================================================================================================
void MyApplication::mainLoop()
{
	if(keys[SDLK_p])
		ragdoll->activate();

	clock.reset();
	physics->mainLoop();

	glLineWidth(2.0);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_LINES);
	for(uint i=0; i<Ragdoll::BP_NUM; i++)
	{
		glColor3f(1, 1, 1);
		glVertex3f(init.heads[i].getX(), init.heads[i].getY(), init.heads[i].getZ());
		glColor3f(1, 0, 1);
		glVertex3f(init.tails[i].getX(), init.tails[i].getY(), init.tails[i].getZ());
	}
	glEnd();
	GlStateMachineSingleton::getInstance().setDepthTestEnabled(true);
	glLineWidth(1.0);
}


//======================================================================================================================
// initAdditionalPhysics                                                                                               =
//======================================================================================================================
void MyApplication::initAdditionalPhysics()
{
	physics->dynamicsWorld->setGravity(btVector3(0, 0, 0));

	// ground
	btCollisionShape* groundShape = new btBoxShape(btVector3(50.0, 50.0, 50.0));
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0.0, -50.0, 0.0));
	physics->createNewRigidBody(0.0, groundTransform, groundShape);

	// ragdoll
	init.heads[Ragdoll::BP_HEAD] = btVector3(-2.48388430727e-15, -0.0822981148958, 2.4998857975);
	init.tails[Ragdoll::BP_HEAD] = btVector3(-1.88069414608e-15, -0.540342450142, 3.00129938126);
	init.heads[Ragdoll::BP_SPINE_UP] = btVector3(0.0, 0.156018421054, 1.59865832329);
	init.tails[Ragdoll::BP_SPINE_UP] = btVector3(-2.48388981299e-15, -0.0822980701923, 2.49988484383);
	init.heads[Ragdoll::BP_SPINE_DOWN] = btVector3(0.0, 0.178849726915, 1.23423588276);
	init.tails[Ragdoll::BP_SPINE_DOWN] = btVector3(0.0, 0.156018406153, 1.59865832329);
	init.heads[Ragdoll::BP_PELVIS] = btVector3(0.0, 0.174057617784, 0.53321672678);
	init.tails[Ragdoll::BP_PELVIS] = btVector3(0.0, 0.178849726915, 1.23423588276);
	init.heads[Ragdoll::BP_ARM_L_UP] = btVector3(-0.673257708549, 0.156014889479, 2.07110834122);
	init.tails[Ragdoll::BP_ARM_L_UP] = btVector3(-1.32804703712, 0.380398094654, 1.87382745743);
	init.heads[Ragdoll::BP_ARM_L_DOWN] = btVector3(-1.32804691792, 0.380398094654, 1.87382757664);
	init.tails[Ragdoll::BP_ARM_L_DOWN] = btVector3(-2.37290859222, -0.101908236742, 1.73465061188);
	init.heads[Ragdoll::BP_HAND_L] = btVector3(-2.37290859222, -0.101908236742, 1.73465061188);
	init.tails[Ragdoll::BP_HAND_L] = btVector3(-3.13194203377, -0.25, 1.8);
	init.heads[Ragdoll::BP_LEG_L_UP] = btVector3(-0.298296809196, -1.19209289551e-07, 0.51272559166);
	init.tails[Ragdoll::BP_LEG_L_UP] = btVector3(-0.470431149006, -0.210588589311, -0.811400771141);
	init.heads[Ragdoll::BP_LEG_L_DOWN] = btVector3(-0.470431268215, -0.210588634014, -0.811400771141);
	init.tails[Ragdoll::BP_LEG_L_DOWN] = btVector3(-0.664484083652, 0.274065643549, -2.70585846901);
	init.heads[Ragdoll::BP_FOOT_L] = btVector3(-0.664484083652, 0.274065703154, -2.70585870743);
	init.tails[Ragdoll::BP_FOOT_L] = btVector3(-0.634733438492, -0.276972800493, -3.02026200294);

	uint rights[] = {Ragdoll::BP_ARM_R_UP, Ragdoll::BP_ARM_R_DOWN, Ragdoll::BP_HAND_R, Ragdoll::BP_LEG_R_UP,
	                 Ragdoll::BP_LEG_R_DOWN, Ragdoll::BP_FOOT_R};
	uint lefts[] = {Ragdoll::BP_ARM_L_UP, Ragdoll::BP_ARM_L_DOWN, Ragdoll::BP_HAND_L, Ragdoll::BP_LEG_L_UP,
	                Ragdoll::BP_LEG_L_DOWN, Ragdoll::BP_FOOT_L};
	for(uint i=0; i<sizeof(rights)/sizeof(uint); i++)
	{
		init.heads[rights[i]] = init.heads[lefts[i]] * btVector3(-1, 1, 1);
		init.tails[rights[i]] = init.tails[lefts[i]] * btVector3(-1, 1, 1);
	}

	btMatrix3x3 rot(btQuaternion(btVector3(1, 0, 0), -PI/2));
	btTransform trf(rot, btVector3(0, 3, 0));
	for(uint i=0; i<Ragdoll::BP_NUM; i++)
	{
		init.heads[i] = trf * init.heads[i];
		init.tails[i] = trf * init.tails[i];
	}

	ragdoll = new Ragdoll(*physics, init);
}

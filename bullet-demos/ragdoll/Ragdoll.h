#ifndef RAGDOLL_H
#define RAGDOLL_H

#include <iostream>
#include <LinearMath/btVector3.h>

class Physics;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;


/**
 *
 */
class Ragdoll
{
	public:
		/**
		 * @note Keep the rights and lefts separate
		 */
		enum BodyPart
		{
			BP_HEAD,
			BP_SPINE_UP,
			BP_SPINE_DOWN,
			BP_PELVIS,
			BP_ARM_L_UP,
			BP_ARM_L_DOWN,
			BP_HAND_L,
			BP_LEG_L_UP,
			BP_LEG_L_DOWN,
			BP_FOOT_L,
			BP_ARM_R_UP,
			BP_ARM_R_DOWN,
			BP_HAND_R,
			BP_LEG_R_UP,
			BP_LEG_R_DOWN,
			BP_FOOT_R,
			BP_NUM
		};
		
		enum Joints
		{
			J_NECK,
			J_SPINE_UP,
			J_SPINE_DOWN,
			J_SHOULDER_L,
			J_ELBOW_L,
			J_WRIST_L,
			J_SHOULDER_R,
			J_ELBOW_R,
			J_WRIST_R,
			J_HIP_L,
			J_KNEE_L,
			J_ANKLE_L,
			J_HIP_R,
			J_KNEE_R,
			J_ANKLE_R,
			J_NUM
		};
		
		struct Initializer
		{
			btVector3 heads[Ragdoll::BP_NUM];
			btVector3 tails[Ragdoll::BP_NUM];

			Initializer();
		};

		Ragdoll(Physics& physics, const Initializer& init);
		void activate();

	private:
		Physics& physics; ///< Know your father
		btCollisionShape* shapes[BP_NUM];
		btRigidBody* bodies[BP_NUM];
		btTypedConstraint* joints[J_NUM];
};


inline Ragdoll::Initializer::Initializer()
{}


#endif

#ifndef PARTICLEEMITER_H
#define PARTICLEEMITER_H

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <vector>
#include "Application.h"
#include <SDL/SDL.h>


using namespace std;

extern Application* app;


float randRange(float min, float max)
{
	float r = (float)rand() / (float)RAND_MAX;
	return min + r * (max - min);
}


float randFloat(float max)
{
	float r = float(rand()) / float(RAND_MAX);
	return r * max;
}


class SceneNode
{
	public:
		btTransform worldTransform;
};


class ParticleEmitterProps
{
	public:
		// particle props
		float particleLife; ///< Required
		float particleLifeMargin;

		btVector3 forceDirection; ///< Required
		btVector3 forceDirectionMargin;
		float forceMagnitude; ///< Required
		float forceMagnitudeMargin;

		float particleMass; ///< Required
		float particleMassMargin;

		bool usingWorldGrav;
		btVector3 gravity; ///< If not set then it uses the world's default
		btVector3 gravityMargin;

		btVector3 startingPos; ///< If not set the default is zero
		btVector3 startingPosMargin;

		// the emittion properties
		uint maxNumOfParticles; ///< The size of the particles vector. Required
		float emittionPeriod; ///< How often the emitter emits new particles. In ms. Required
		uint particlesPerEmittion; ///< How many particles are emitted every emittion. Required

		ParticleEmitterProps():
			particleLifeMargin(0.0),
			forceDirectionMargin(0.0, 0.0, 0.0),
			forceMagnitudeMargin(0.0),
			particleMassMargin(0.0),
			gravityMargin(0.0, 0.0, 0.0),
			startingPos(0.0, 0.0, 0.0),
			startingPosMargin(0.0, 0.0, 0.0),
			usingWorldGrav(true)
		{}
};


class ParticleEmitter: public SceneNode, public ParticleEmitterProps
{
	public:
		class Particle
		{
			public:
				float timeOfDeath; ///< In seconds, < 0.0 if dead
				btRigidBody* body;

				Particle():
					timeOfDeath(-1.0)
				{}
		};

		// the changeable vars
		vector<Particle*> particles;
		float timeOfPrevUpdate;
		float timeOfPrevEmittion;

		/**
		 * Init the emitter
		 */
		void init()
		{
			// dummy props init
			particleLife = 7.0;
			particleLifeMargin = 2.0;

			forceDirection = btVector3(0.0, 0.000001, 0.0);
			forceDirectionMargin = btVector3(1.0, 0.0, 1.0);
			forceMagnitude = 200.0;
			forceMagnitudeMargin = 100.0;

			particleMass = 1.0;
			particleMassMargin = 0.0;

			usingWorldGrav = false;
			gravity = btVector3(0.0, 10.0, 0.0);
			gravityMargin = btVector3(0.0, 0.0, 0.0);

			startingPos = btVector3(0.0, 1.0, 0.0);
			startingPosMargin = btVector3(0.0, 0.0, 0.0);
			maxNumOfParticles = 50;
			emittionPeriod = 0.05;
			particlesPerEmittion = 2;

			// corrections
			//forceDirection.normalize();

			// sanity checks
			/*if(particleLife - particleLifeMargin <= 0.0 || particleLifeMargin <= 0)
				cerr << "Incorrect value for particleLifeMargin" << endl;

			if(particleMass - particleMassMargin <= 0.0 || particleMassMargin <= 0)
				cerr << "Incorrect value for particleMassMargin" << endl;*/

			// init the particles
			//btCollisionShape* colShape = new btBoxShape(btVector3(0.5, 0.5, 0.5));
			btCollisionShape* colShape = new btSphereShape(0.5);
			btTransform startTransform;
			startTransform.setIdentity();
			btVector3 o = btVector3(0.0, 1.0, 0.0);
			startTransform.setOrigin(o);

			particles.resize(maxNumOfParticles);
			for(uint i = 0; i < maxNumOfParticles; i++)
			{
				particles[i] = new Particle;
				float mass = particleMass + randFloat(particleMassMargin)*2.0 - particleMassMargin;

				/*btVector3 origin = startTransform.getOrigin();
				origin.setX( origin.getX() + 0.6*2.0);
				startTransform.setOrigin(origin);*/

				btRigidBody* body = app->physics->createNewRigidBody(mass, startTransform, colShape, Physics::CG_PARTICLE,
				                                                     Physics::CG_ALL^Physics::CG_PARTICLE);
				body->forceActivationState(DISABLE_SIMULATION);
				//body->setActivationState(ISLAND_SLEEPING);
				particles[i]->body = body;
			}
		}


		/**
		 * update
		 */
		void update()
		{
			float crntTime = SDL_GetTicks() / 1000.0;

			//cout << "Updating " << crntTime << endl;

			// deactivate the dead particles
			for(uint i=0; i<particles.size(); i++)
			{
				Particle& p = *particles[i];
				if(p.timeOfDeath < 0.0) continue; // its already dead so dont deactivate it again

				if(p.timeOfDeath < crntTime)
				{
					//cout << "Killing " << i << " " << p.timeOfDeath << endl;
					p.body->setActivationState(DISABLE_SIMULATION);
					p.timeOfDeath = -1.0;
				}
			}


			if((crntTime - timeOfPrevEmittion) > emittionPeriod)
			{
				uint partNum = 0;
				for(uint i=0; i<particles.size(); i++)
				{
					Particle& p = *particles[i];
					if(p.timeOfDeath > 0.0) continue; // its alive so skip it

					float rf = float(rand()) / float(RAND_MAX);

					//
					// reinit a dead particle
					//
					//cout << "Re-initing " << i << endl;

					// life
					if(particleLifeMargin != 0.0)
						p.timeOfDeath = crntTime + particleLife + randFloat(particleLifeMargin) * 2.0 - particleLifeMargin;
					else
						p.timeOfDeath = crntTime + particleLife;

					//cout << "Time of death " << p.timeOfDeath << endl;
					//cout << "Particle life " << p.timeOfDeath - crntTime << endl;

					// activate it (Bullet stuff)
					p.body->forceActivationState(ACTIVE_TAG);
					p.body->activate();
					//p.body->setDeactivationTime(0);
					p.body->clearForces();
					p.body->setLinearVelocity(btVector3(0.0, 0.0, 0.0));
					p.body->setAngularVelocity(btVector3(0.0, 0.0, 0.0));

					//cout << p.body->internalGetDeltaAngularVelocity() << endl;

					// force
					btVector3 forceDir;
					if(forceDirectionMargin != btVector3(0.0, 0.0, 0.0))
					{
						for(int i=0; i<3; i++)
						{
							forceDir[i] = forceDirection[i] + randFloat(forceDirectionMargin[i]) * 2.0 - forceDirectionMargin[i];
						}
					}
					else
					{
						forceDir = forceDirection;
					}
					forceDir.normalize();
					btVector3 force;

					if(forceMagnitudeMargin != 0.0)
						force = forceDir * (forceMagnitude + randFloat(forceMagnitudeMargin) * 2.0 - forceMagnitudeMargin);
					else
						force = forceDir * forceMagnitude;

					p.body->applyCentralForce(force);

					//cout << p.body->getTotalForce() << endl;

					// gravity
					if(!usingWorldGrav)
					{
						btVector3 grav;
						if(gravityMargin != btVector3(0.0, 0.0, 0.0))
						{
							for(int i=0; i<3; i++)
							{
								grav[i] = gravity[i] + randFloat(gravityMargin[i]) * 2.0 - gravityMargin[i];
							}
						}
						else
						{
							grav = gravity;
						}
						p.body->setGravity(grav);
					}

					// starting pos
					btVector3 pos;
					if(startingPosMargin != btVector3(0.0, 0.0, 0.0))
					{
						for(int i=0; i<3; i++)
						{
							pos[i] = startingPos[i] + randFloat(startingPosMargin[i]) * 2.0 - startingPosMargin[i];
						}
					}
					else
					{
						pos = startingPos;
					}
					pos += worldTransform.getOrigin();
					btTransform trf;
					trf.setIdentity();
					trf.setOrigin(pos);
					p.body->setWorldTransform(trf);

					//cout << p.body->getLinearVelocity() << endl;

					// do the rest
					++partNum;
					if(partNum >= particlesPerEmittion) break;
				} // end for all particles

				timeOfPrevEmittion = crntTime;
			} // end if can emit

			timeOfPrevUpdate = crntTime;
		}


	private:
};

#endif

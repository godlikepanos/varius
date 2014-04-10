#ifndef DEBUGDRAW_H
#define DEBUGDRAW_H

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>


using namespace std;


/**
 * Custom Bullet debug class
 */
class DebugDraw: public btIDebugDraw
{
	public:
		DebugDraw();

	private:
		int debugMode;
		int sphereComplexity;

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
		void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime,
		                      const btVector3& color) {}
		void drawSphere(btScalar radius, const btTransform& transform, const btVector3& color);
		void reportErrorWarning(const char* warningString);
		void draw3dText(const btVector3& location, const char *textString);
		void setDebugMode(int debugMode_);
		int getDebugMode() const;
};


inline DebugDraw::DebugDraw():
	sphereComplexity(8)
{}


inline void DebugDraw::reportErrorWarning(const char* warningString)
{
	cerr << warningString << endl;
}


inline void DebugDraw::draw3dText(const btVector3& location, const char *textString)
{
	cout << textString << endl;
}


inline void DebugDraw::setDebugMode(int debugMode_)
{
	debugMode = debugMode_;
}


inline int DebugDraw::getDebugMode() const
{
	return debugMode;
}


#endif

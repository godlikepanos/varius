#include <cmath>
#include "DebugDraw.h"


//======================================================================================================================
// drawLine                                                                                                            =
//======================================================================================================================
void DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	glColor3fv(&color[0]);
	glBegin(GL_LINES);
	glVertex3fv(&from[0]);
	glVertex3fv(&to[0]);
	glEnd();
}


//======================================================================================================================
// drawSphere                                                                                                          =
//======================================================================================================================
void DebugDraw::drawSphere(btScalar radius, const btTransform& transform, const btVector3& color)
{
	const float PI = 3.14159265358979323846;
	const float twopi  = PI * 2;
	const float pidiv2 = PI / 2;

	btAlignedObjectArray<btVector3> positions;
	positions.reserve(sphereComplexity / 2 * sphereComplexity);

	for(int i = 0; i < sphereComplexity / 2; ++i)
	{
		float theta1 = i * twopi / sphereComplexity - pidiv2;
		float theta2 = (i + 1) * twopi / sphereComplexity - pidiv2;

		for(int j = sphereComplexity; j >= 0; --j)
		{
			float theta3 = j * twopi / sphereComplexity;

			float sintheta1 = sin(theta1);
			float costheta1 = cos(theta1);
			float sintheta2 = sin(theta2);
			float costheta2 = cos(theta2);
			float sintheta3 = sin(theta3);
			float costheta3 = cos(theta3);

			float ex = costheta2 * costheta3;
			float ey = sintheta2;
			float ez = costheta2 * sintheta3;
			float px = radius * ex;
			float py = radius * ey;
			float pz = radius * ez;


			positions.push_back(/*transform **/ btVector3(px, py, pz));

			ex = costheta1 * costheta3;
			ey = sintheta1;
			ez = costheta1 * sintheta3;
			px = radius * ex;
			py = radius * ey;
			pz = radius * ez;

			positions.push_back(/*transform **/ btVector3(px, py, pz));
		}
	}

	float mat[16];
	transform.getOpenGLMatrix(mat);
	glPushMatrix();
	glMultMatrixf(mat);

	glBegin(GL_LINES);
	glColor3fv(&color[0]);
	for(int i=0; i<positions.size()/2-1; i++)
	{
		int v0 = 2*i;
		int v1 = 2*i+1;
		int v2 = 2*i+3;
		/*drawLine(positions[v0], positions[v1], color);
		drawLine(positions[v1], positions[v2], color);*/

		glVertex3fv(&positions[v0][0]);
		glVertex3fv(&positions[v1][0]);
		glVertex3fv(&positions[v1][0]);
		glVertex3fv(&positions[v2][0]);
	}
	glEnd();
	/*glPolygonMode(GL_FRONT, GL_LINE);
	glColor3fv(&color[0]);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, &positions[0][0]);
	glDrawArrays(GL_QUAD_STRIP, 0, positions.size());
	glDisableClientState(GL_VERTEX_ARRAY);*/
	glPopMatrix();
}


#ifndef VEC4_H
#define VEC4_H

#include <iostream>


using namespace std;
class Vec3;


class Vec4
{
	public:
		class Nested
		{
			public:
				void foo() {cout << "Fooska" << endl;}
		};

		static Vec4& getInstance()
		{
			static Vec4* instance = new Vec4(1, 2, 3, 4);
			return *instance;
		}

		float x;
		float y;
		float z;
		float w;
		Nested nested;

		Vec4() {}
		Vec4(const Vec4& v);
		Vec4(float a);
		Vec4(float x, float y, float z, float w);
		Vec4(const Vec3& v, float w);
};


#include "Vec3.h"


inline Vec4::Vec4(const Vec4& v):
	x(v.x),
	y(v.y),
	z(v.z),
	w(v.w)
{}


inline Vec4::Vec4(float a):
	x(a),
	y(a),
	z(a),
	w(a)
{}


inline Vec4::Vec4(float x, float y, float z, float w):
	x(x),
	y(y),
	z(z),
	w(w)
{}


inline Vec4::Vec4(const Vec3& v, float w):
	x(v.x),
	y(v.y),
	z(v.z),
	w(w)
{}


#endif

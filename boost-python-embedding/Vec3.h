#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <string>


class Vec4;


class Vec3
{
	public:
		float x;
		float y;
		float z;

		Vec3() {}
		Vec3(const Vec3& v);
		Vec3(float a);
		Vec3(float x, float y, float z);
		Vec3(const Vec4& v);

		const std::string& getStr() const {return str;}
		std::string& getStr() {return str;}

	private:
		std::string str;
};


#include "Vec4.h"


inline Vec3::Vec3(const Vec3& v):
	x(v.x),
	y(v.y),
	z(v.z)
{}


inline Vec3::Vec3(float a):
	x(a),
	y(a),
	z(a)
{}


inline Vec3::Vec3(float x, float y, float z):
	x(x),
	y(y),
	z(z)
{}


inline Vec3::Vec3(const Vec4& v):
	x(v.x),
	y(v.y),
	z(v.z)
{}


#endif

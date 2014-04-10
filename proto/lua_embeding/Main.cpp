// To empty the stack: lua_settop(l, 0)
// Get the stack size: lua_gettop(l)

#include "Foo.hpp"
#include "Vec2.h"
#include <lua.hpp>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <typeinfo>
#include <cassert>
#include <cstring>
#include <unordered_map>
#include <functional>
#include "LuaBinder.h"

using namespace anki;

static std::string readFile(const char* filename)
{
	std::ifstream file(filename);
	if(!file.is_open())
	{
		throw std::runtime_error("Cannot open file");
	}

	return std::string((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());
}

void wrapFoo(LuaBinder& lb)
{
	ANKI_LUA_CLASS_BEGIN(lb, Foo)
		ANKI_LUA_CONSTRUCTOR(float, float)
		ANKI_LUA_STATIC_METHOD("getZero", &Foo::getZero)
		ANKI_LUA_METHOD("methodii", &Foo::methodii)
		ANKI_LUA_METHOD("methodstr", &Foo::methodstr)
		ANKI_LUA_METHOD("methodargfoo", &Foo::methodargfoo)
		ANKI_LUA_METHOD("copy", &Foo::operator=)
	ANKI_LUA_CLASS_END()
}

void vec2SetX(Vec2* this_, float x)
{
	this_->x() = x;
}

float vec2GetX(const Vec2* this_)
{
	return this_->x();
}

void wrapVec2(LuaBinder& lb)
{
	ANKI_LUA_CLASS_BEGIN(lb, Vec2)
		ANKI_LUA_CONSTRUCTOR(float, float)
		ANKI_LUA_METHOD("print", &Vec2::print)
		ANKI_LUA_FUNCTION_AS_METHOD("setX", &vec2SetX)
		ANKI_LUA_FUNCTION_AS_METHOD("getX", &vec2GetX)
	ANKI_LUA_CLASS_END()
}

int main(int, char**)
{
	LuaBinder lb;

	wrapFoo(lb);
	wrapVec2(lb);

	std::cout << "===============================================" << std::endl;
	lb.evalString(readFile("test.lua").c_str());
	std::cout << "===============================================" << std::endl;

	return 0;
}



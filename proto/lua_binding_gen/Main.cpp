#include <cstdio>
#include <cstddef>
#include <lua.hpp>
#include <new>
#include <cassert>
#include <algorithm>

struct UserData
{
	void* m_data;
	bool m_gc;
};

#define ANKI_ASSERT assert
using Error = int;

void checkArgsCount(lua_State* l, int argsCount)
{
	int actualArgsCount = lua_gettop(l);
	if(argsCount != actualArgsCount)
	{
		luaL_error(l, "Expecting %d arguments, got %d", argsCount, 
			actualArgsCount);
	}
}

void createClass(lua_State* l, const char* className)
{
	lua_newtable(l); // push new table
	lua_setglobal(l, className); // pop and make global
	luaL_newmetatable(l, className); // push
	lua_pushstring(l, "__index"); // push
	lua_pushvalue(l, -2);  // pushes copy of the metatable
	lua_settable(l, -3);  // pop*2: metatable.__index = metatable

	// After all these the metatable is in the top of tha stack
}

void pushLuaCFuncMethod(lua_State* l, const char* name, lua_CFunction luafunc)
{
	lua_pushstring(l, name);
	lua_pushcfunction(l, luafunc);
	lua_settable(l, -3);
}

void pushLuaCFuncStaticMethod(lua_State* l, const char* className,
	const char* name, lua_CFunction luafunc) 
{
	lua_getglobal(l, className); // push
	lua_pushcfunction(l, luafunc); // push
	lua_setfield(l, -2, name); // pop global
	lua_pop(l, 1); // pop cfunc
}

void* luaAlloc(lua_State* l, size_t size)
{
	void* ud;
	lua_Alloc alloc = lua_getallocf(l, &ud);

	return alloc(ud, nullptr, 0, size);
}

void luaFree(lua_State* l, void* ptr)
{
	void* ud;
	lua_Alloc alloc = lua_getallocf(l, &ud);

	alloc(ud, ptr, 0, 0);
}

#define INFO() printf("  %s\n", __PRETTY_FUNCTION__)

class Foo
{
public:
	int m_x;

	Foo(int x)
	:	m_x(x)
	{
		INFO();
	}

	Foo(const Foo& f)
	:	m_x(f.m_x)
	{
		INFO();
	}

	Foo(Foo&& f)
	:	m_x(f.m_x)
	{
		f.m_x = 0;
		INFO();
	}

	~Foo()
	{
		INFO();
	}

	Foo& operator=(const Foo& b)
	{
		m_x = b.m_x;
		return *this;
	}

	void simplePrint()
	{
		printf("Hello %d\n", m_x);
	}

	int x()
	{
		return m_x;
	}

	template<typename Number>
	Number alias()
	{
		return m_x;
	}

	Error returnError(int err)
	{
		return err;
	}

	const char* returnString() const
	{
		return "a string";
	}

	static int staticMethod(Foo& f, int x)
	{
		f.m_x += x;
		return f.m_x;
	}

	Foo operator+(const Foo& b) const
	{
		return Foo(m_x + b.m_x);
	}

	int& operator()(int i, int j)
	{
		return m_x;
	}

	int operator()(int i, int j) const
	{
		return m_x;
	}
};

class Boo
{
public:
	Foo m_foo;

	Boo(const Foo& f)
	:	m_foo(f)
	{
		INFO();
	}

	~Boo()
	{
		INFO();
	}

	Foo get() const
	{
		return m_foo;
	}

	Foo& getRef()
	{
		return m_foo;
	}

	Foo* getPtr()
	{
		return &m_foo;
	}

	int manyArgs(int x, float f, Foo foo, const Foo& foo1)
	{
		printf("Many args %d %f\n", x, f);
		return x + f;
	}
};

#include "FooAutogen.h"

lua_State* l;

int cfunc(lua_State* l)
{
	lua_pushstring(l, "lalala");
	//lua_error(l);
	return 1;
}

const char* source = R"(
local a = Foo.new(1)
local b = Foo.new(2)
local c = a + b
print(c:x())
)";

int main(int, char**)
{
	l = luaL_newstate();
	luaL_openlibs(l);

	lua_register(l, "cfunc", cfunc);
	wrapModuleFoo(l);

	//printf("mem %d\n", lua_gc(l, LUA_GCCOUNT, 0));
	int err = luaL_dostring(l, source);
	if(err)
	{
		printf("LUA ERROR: %s\n", lua_tostring(l, -1));
		lua_pop(l, 1);
	}
	//printf("mem %d\n", lua_gc(l, LUA_GCCOUNT, 0));
	printf("-------- GC\n");
	lua_gc(l, LUA_GCCOLLECT, 0);
	//printf("mem %d\n", lua_gc(l, LUA_GCCOUNT, 0));

	lua_close(l);

	return 0;
}



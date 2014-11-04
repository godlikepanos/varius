#include <cstdio>
#include <cstddef>
#include <lua.hpp>
#include <new>

struct UserData
{
	void* m_data;
	bool m_gc;
};

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

class Foo
{
public:
	int m_x;

	Foo(int x)
	:	m_x(x)
	{}

	~Foo()
	{
		printf("~Foo\n");
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
local foo = Foo.new(123)
foo:simplePrint()
print(string.format("x is:%d", foo:x()))
print(foo:aliasFloat())
)";

int main(int, char**)
{
	l = luaL_newstate();
	luaL_openlibs(l);

	lua_register(l, "cfunc", cfunc);
	wrapModuleFoo(l);

	printf("mem %d\n", lua_gc(l, LUA_GCCOUNT, 0));
	int err = luaL_dostring(l, source);
	if(err)
	{
		printf("LUA ERROR: %s\n", lua_tostring(l, -1));
		lua_pop(l, 1);
	}
	printf("mem %d\n", lua_gc(l, LUA_GCCOUNT, 0));
	lua_gc(l, LUA_GCCOLLECT, 0);
	printf("mem %d\n", lua_gc(l, LUA_GCCOUNT, 0));

	lua_close(l);

	return 0;
}



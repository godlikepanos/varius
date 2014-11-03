#include <cstdio>
#include <cstddef>
#include <lua.hpp>

struct UserData
{
	void* m_data;
	bool m_gc;
};

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

class Foo
{
public:
	void simplePrint()
	{
		printf("Hello\n");
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
foo = Foo.new()
foo:simplePrint()
)";

int main(int, char**)
{
	l = luaL_newstate();
	luaL_openlibs(l);

	lua_register(l, "cfunc", cfunc);
	wrapModuleFoo(l);

	int err = luaL_dostring(l, source);
	if(err)
	{
		printf("LUA ERROR: %s\n", lua_tostring(l, -1));
		lua_pop(l, 1);
	}

	return 0;
}



#include <cstdio>
#include <lua.hpp>

lua_State* l;

int cfunc(lua_State* l)
{
	lua_pushstring(l, "lalala");
	//lua_error(l);
	return 1;
}

int main(int, char**)
{
	l = luaL_newstate();
	luaL_openlibs(l);

	lua_register(l, "cfunc", cfunc);

	int err = luaL_dostring(l, "print(cfunc()) print('new')");
	if(err)
	{
		printf("LUA ERROR: %s\n", lua_tostring(l, -1));
		lua_pop(l, 1);
	}

	return 0;
}



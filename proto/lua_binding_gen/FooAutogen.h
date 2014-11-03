static const char* classnameFoo = "Foo";

// Constructor for Foo
static int wrapFooCtor(lua_State* l)
{
	void* ptr = lua_newuserdata(l, sizeof(UserData));
	luaL_setmetatable(l, classnameFoo);
	
	Foo* inst = new Foo();
	if(inst == nullptr)
	{
		lua_pushstring(l, "Out of memory");
		lua_error(l);
	}
	
	UserData* ud = reinterpret_cast<UserData*>(ptr);
	ud->m_data = inst;
	ud->m_gc = true;
	
	return 1;
}

// Method Foo::simplePrint
static int wrapFoosimplePrint(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	
	self->simplePrint();
	
	return 0;
}

static void wrapFoo(lua_State* l)
{
	createClass(l, "Foo");
	pushLuaCFuncStaticMethod(l, classnameFoo, "new", wrapFooCtor);
	pushLuaCFuncMethod(l, "simplePrint", wrapFoosimplePrint);
	lua_settop(l, 0);
}

void wrapModuleFoo(lua_State* l)
{
	wrapFoo(l);
}


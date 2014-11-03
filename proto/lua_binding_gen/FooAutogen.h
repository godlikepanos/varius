static const char* classnameFoo = "Foo";

// Constructor for Foo
static int wrapFooCtor(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	int arg1(luaL_checknumber(l, 1));
	
	voidp = lua_newuserdata(l, sizeof(UserData));
	luaL_setmetatable(l, classnameFoo);
	
	void* inst = luaAlloc(l, sizeof(Foo));
	if(inst == nullptr)
	{
		lua_pushstring(l, "Out of memory");
		lua_error(l);
	}
	
	::new(inst) Foo(arg1);
	
	ud = reinterpret_cast<UserData*>(voidp);
	ud->m_data = inst;
	ud->m_gc = true;
	
	return 1;
}

// Destructor for Foo
static int wrapFooDtor(lua_State* l)
{
	checkArgsCount(l, 1);
	void* voidp = luaL_checkudata(l, 1, classnameFoo);
	UserData* ud = reinterpret_cast<UserData*>(voidp);
	if(ud->m_gc)
	{
		Foo* inst = reinterpret_cast<Foo*>(ud->m_data);
		inst->~Foo();
		luaFree(l, inst);
	}
	
	return 0;
}

// Method Foo::simplePrint
static int wrapFoosimplePrint(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
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
	pushLuaCFuncMethod(l, "__gc", wrapFooDtor);
	pushLuaCFuncMethod(l, "simplePrint", wrapFoosimplePrint);
	lua_settop(l, 0);
}

void wrapModuleFoo(lua_State* l)
{
	wrapFoo(l);
}


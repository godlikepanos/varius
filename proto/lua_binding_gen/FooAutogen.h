// This is auto generated file

//==============================================================================
// Foo                                                                         =
//==============================================================================

//==============================================================================
static const char* classnameFoo = "Foo";

//==============================================================================
/// Pre-wrap constructor for Foo.
static int pwrapFooCtor(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	// Pop arguments
	int arg0(luaL_checknumber(l, 1));
	
	// Create user data
	voidp = lua_newuserdata(l, sizeof(UserData));
	ud = reinterpret_cast<UserData*>(voidp);
	ud->m_data = nullptr;
	ud->m_gc = false;
	luaL_setmetatable(l, classnameFoo);
	
	void* inst = luaAlloc(l, sizeof(Foo));
	if(inst == nullptr)
	{
		lua_pushstring(l, "Out of memory");
		return -1;
	}
	
	::new(inst) Foo(arg0);
	
	ud->m_data = inst;
	ud->m_gc = true;
	
	return 1;
}

//==============================================================================
/// Wrap constructor for Foo.
static int wrapFooCtor(lua_State* l)
{
	int res = pwrapFooCtor(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Wrap destructor for Foo.
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

//==============================================================================
/// Pre-wrap method Foo::simplePrint.
static int pwrapFoosimplePrint(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Call the method
	self->simplePrint();
	
	return 0;
}

//==============================================================================
/// Wrap method Foo::simplePrint.
static int wrapFoosimplePrint(lua_State* l)
{
	int res = pwrapFoosimplePrint(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Foo::x.
static int pwrapFoox(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Call the method
	int ret = self->x();
	
	// Push return value
	lua_pushnumber(l, ret);
	
	return 1;
}

//==============================================================================
/// Wrap method Foo::x.
static int wrapFoox(lua_State* l)
{
	int res = pwrapFoox(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Foo::alias<float>.
static int pwrapFooaliasFloat(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Call the method
	float ret = self->alias<float>();
	
	// Push return value
	lua_pushnumber(l, ret);
	
	return 1;
}

//==============================================================================
/// Wrap method Foo::alias<float>.
static int wrapFooaliasFloat(lua_State* l)
{
	int res = pwrapFooaliasFloat(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Foo::returnError.
static int pwrapFooreturnError(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 2);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Pop arguments
	int arg0(luaL_checknumber(l, 2));
	
	// Call the method
	Error ret = self->returnError(arg0);
	
	// Push return value
	if(ret)
	{
		lua_pushstring(l, "Glue code returned an error");
		return -1;
	}
	
	lua_pushnumber(l, ret);
	
	return 1;
}

//==============================================================================
/// Wrap method Foo::returnError.
static int wrapFooreturnError(lua_State* l)
{
	int res = pwrapFooreturnError(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Foo::returnString.
static int pwrapFooreturnString(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Call the method
	const char* ret = self->returnString();
	
	// Push return value
	lua_pushstring(l, ret);
	
	return 1;
}

//==============================================================================
/// Wrap method Foo::returnString.
static int wrapFooreturnString(lua_State* l)
{
	int res = pwrapFooreturnString(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap static method Foo::staticMethod.
static int pwrapFoostaticMethod(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 2);
	
	// Pop arguments
	voidp = luaL_checkudata(l, 1, "Foo");
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* iarg0 = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(iarg0 != nullptr);
	Foo& arg0(*iarg0);
	
	int arg1(luaL_checknumber(l, 2));
	
	// Call the method
	int ret = Foo::staticMethod(arg0, arg1);
	
	// Push return value
	lua_pushnumber(l, ret);
	
	return 1;
}

//==============================================================================
/// Wrap static method Foo::staticMethod.
static int wrapFoostaticMethod(lua_State* l)
{
	int res = pwrapFoostaticMethod(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Foo::operator=.
static int pwrapFoocopy(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 2);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Pop arguments
	voidp = luaL_checkudata(l, 2, "Foo");
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* iarg0 = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(iarg0 != nullptr);
	const Foo& arg0(*iarg0);
	
	// Call the method
	Foo& ret = self->operator=(arg0);
	
	// Push return value
	voidp = lua_newuserdata(l, sizeof(UserData));
	ud = reinterpret_cast<UserData*>(voidp);
	luaL_setmetatable(l, "Foo");
	ud->m_data = reinterpret_cast<void*>(&ret);
	ud->m_gc = false;
	
	return 1;
}

//==============================================================================
/// Wrap method Foo::operator=.
static int wrapFoocopy(lua_State* l)
{
	int res = pwrapFoocopy(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Foo::operator+.
static int pwrapFoo__add(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 2);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Pop arguments
	voidp = luaL_checkudata(l, 2, "Foo");
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* iarg0 = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(iarg0 != nullptr);
	const Foo& arg0(*iarg0);
	
	// Call the method
	Foo ret = self->operator+(arg0);
	
	// Push return value
	voidp = lua_newuserdata(l, sizeof(UserData));
	ud = reinterpret_cast<UserData*>(voidp);
	luaL_setmetatable(l, "Foo");
	ud->m_data = luaAlloc(l, sizeof(Foo));
	if(ud->m_data == nullptr)
	{
		ud->m_gc = false;
		lua_pushstring(l, "Out of memory");
		return -1;
	}
	
	::new(ud->m_data) Foo(std::move(ret));
	ud->m_gc = true;
	
	return 1;
}

//==============================================================================
/// Wrap method Foo::operator+.
static int wrapFoo__add(lua_State* l)
{
	int res = pwrapFoo__add(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Foo::operator().
static int pwrapFoogetIJ(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 3);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Pop arguments
	int arg0(luaL_checknumber(l, 2));
	
	int arg1(luaL_checknumber(l, 3));
	
	// Call the method
	int ret = self->operator()(arg0, arg1);
	
	// Push return value
	lua_pushnumber(l, ret);
	
	return 1;
}

//==============================================================================
/// Wrap method Foo::operator().
static int wrapFoogetIJ(lua_State* l)
{
	int res = pwrapFoogetIJ(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Foo::setIJ.
static int pwrapFoosetIJ(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 4);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameFoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* self = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Pop arguments
	int arg0(luaL_checknumber(l, 2));
	
	int arg1(luaL_checknumber(l, 3));
	
	int arg2(luaL_checknumber(l, 4));
	
	// Call the method
	(*self)(arg0, arg1) = arg2;
	
	return 0;
}

//==============================================================================
/// Wrap method Foo::setIJ.
static int wrapFoosetIJ(lua_State* l)
{
	int res = pwrapFoosetIJ(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Wrap class Foo.
static void wrapFoo(lua_State* l)
{
	createClass(l, classnameFoo);
	pushLuaCFuncStaticMethod(l, classnameFoo, "new", wrapFooCtor);
	pushLuaCFuncMethod(l, "__gc", wrapFooDtor);
	pushLuaCFuncMethod(l, "simplePrint", wrapFoosimplePrint);
	pushLuaCFuncMethod(l, "x", wrapFoox);
	pushLuaCFuncMethod(l, "aliasFloat", wrapFooaliasFloat);
	pushLuaCFuncMethod(l, "returnError", wrapFooreturnError);
	pushLuaCFuncMethod(l, "returnString", wrapFooreturnString);
	pushLuaCFuncStaticMethod(l, classnameFoo, "staticMethod", wrapFoostaticMethod);
	pushLuaCFuncMethod(l, "copy", wrapFoocopy);
	pushLuaCFuncMethod(l, "__add", wrapFoo__add);
	pushLuaCFuncMethod(l, "getIJ", wrapFoogetIJ);
	pushLuaCFuncMethod(l, "setIJ", wrapFoosetIJ);
	lua_settop(l, 0);
}

//==============================================================================
// Boo                                                                         =
//==============================================================================

//==============================================================================
static const char* classnameBoo = "Boo";

//==============================================================================
/// Pre-wrap constructor for Boo.
static int pwrapBooCtor(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	// Pop arguments
	voidp = luaL_checkudata(l, 1, "Foo");
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* iarg0 = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(iarg0 != nullptr);
	const Foo& arg0(*iarg0);
	
	// Create user data
	voidp = lua_newuserdata(l, sizeof(UserData));
	ud = reinterpret_cast<UserData*>(voidp);
	ud->m_data = nullptr;
	ud->m_gc = false;
	luaL_setmetatable(l, classnameBoo);
	
	void* inst = luaAlloc(l, sizeof(Boo));
	if(inst == nullptr)
	{
		lua_pushstring(l, "Out of memory");
		return -1;
	}
	
	::new(inst) Boo(arg0);
	
	ud->m_data = inst;
	ud->m_gc = true;
	
	return 1;
}

//==============================================================================
/// Wrap constructor for Boo.
static int wrapBooCtor(lua_State* l)
{
	int res = pwrapBooCtor(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Wrap destructor for Boo.
static int wrapBooDtor(lua_State* l)
{
	checkArgsCount(l, 1);
	void* voidp = luaL_checkudata(l, 1, classnameBoo);
	UserData* ud = reinterpret_cast<UserData*>(voidp);
	if(ud->m_gc)
	{
		Boo* inst = reinterpret_cast<Boo*>(ud->m_data);
		inst->~Boo();
		luaFree(l, inst);
	}
	
	return 0;
}

//==============================================================================
/// Pre-wrap method Boo::get.
static int pwrapBooget(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameBoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Boo* self = reinterpret_cast<Boo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Call the method
	Foo ret = self->get();
	
	// Push return value
	voidp = lua_newuserdata(l, sizeof(UserData));
	ud = reinterpret_cast<UserData*>(voidp);
	luaL_setmetatable(l, "Foo");
	ud->m_data = luaAlloc(l, sizeof(Foo));
	if(ud->m_data == nullptr)
	{
		ud->m_gc = false;
		lua_pushstring(l, "Out of memory");
		return -1;
	}
	
	::new(ud->m_data) Foo(std::move(ret));
	ud->m_gc = true;
	
	return 1;
}

//==============================================================================
/// Wrap method Boo::get.
static int wrapBooget(lua_State* l)
{
	int res = pwrapBooget(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Boo::getRef.
static int pwrapBoogetRef(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameBoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Boo* self = reinterpret_cast<Boo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Call the method
	Foo& ret = self->getRef();
	
	// Push return value
	voidp = lua_newuserdata(l, sizeof(UserData));
	ud = reinterpret_cast<UserData*>(voidp);
	luaL_setmetatable(l, "Foo");
	ud->m_data = reinterpret_cast<void*>(&ret);
	ud->m_gc = false;
	
	return 1;
}

//==============================================================================
/// Wrap method Boo::getRef.
static int wrapBoogetRef(lua_State* l)
{
	int res = pwrapBoogetRef(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Boo::getPtr.
static int pwrapBoogetPtr(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 1);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameBoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Boo* self = reinterpret_cast<Boo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Call the method
	Foo* ret = self->getPtr();
	
	// Push return value
	voidp = lua_newuserdata(l, sizeof(UserData));
	ud = reinterpret_cast<UserData*>(voidp);
	luaL_setmetatable(l, "Foo");
	ud->m_data = reinterpret_cast<void*>(ret);
	ud->m_gc = false;
	
	return 1;
}

//==============================================================================
/// Wrap method Boo::getPtr.
static int wrapBoogetPtr(lua_State* l)
{
	int res = pwrapBoogetPtr(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Pre-wrap method Boo::manyArgs.
static int pwrapBoomanyArgs(lua_State* l)
{
	UserData* ud;
	(void)ud;
	void* voidp;
	(void)voidp;
	
	checkArgsCount(l, 5);
	
	// Get "this" as "self"
	voidp = luaL_checkudata(l, 1, classnameBoo);
	ud = reinterpret_cast<UserData*>(voidp);
	Boo* self = reinterpret_cast<Boo*>(ud->m_data);
	ANKI_ASSERT(self != nullptr);
	
	// Pop arguments
	int arg0(luaL_checknumber(l, 2));
	
	float arg1(luaL_checknumber(l, 3));
	
	voidp = luaL_checkudata(l, 4, "Foo");
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* iarg2 = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(iarg2 != nullptr);
	Foo arg2(*iarg2);
	
	voidp = luaL_checkudata(l, 5, "Foo");
	ud = reinterpret_cast<UserData*>(voidp);
	Foo* iarg3 = reinterpret_cast<Foo*>(ud->m_data);
	ANKI_ASSERT(iarg3 != nullptr);
	const Foo& arg3(*iarg3);
	
	// Call the method
	int ret = self->manyArgs(arg0, arg1, arg2, arg3);
	
	// Push return value
	lua_pushnumber(l, ret);
	
	return 1;
}

//==============================================================================
/// Wrap method Boo::manyArgs.
static int wrapBoomanyArgs(lua_State* l)
{
	int res = pwrapBoomanyArgs(l);
	if(res >= 0) return res;
	lua_error(l);
	return 0;
}

//==============================================================================
/// Wrap class Boo.
static void wrapBoo(lua_State* l)
{
	createClass(l, classnameBoo);
	pushLuaCFuncStaticMethod(l, classnameBoo, "new", wrapBooCtor);
	pushLuaCFuncMethod(l, "__gc", wrapBooDtor);
	pushLuaCFuncMethod(l, "get", wrapBooget);
	pushLuaCFuncMethod(l, "getRef", wrapBoogetRef);
	pushLuaCFuncMethod(l, "getPtr", wrapBoogetPtr);
	pushLuaCFuncMethod(l, "manyArgs", wrapBoomanyArgs);
	lua_settop(l, 0);
}

//==============================================================================
/// Wrap the module.
void wrapModuleFoo(lua_State* l)
{
	wrapFoo(l);
	wrapBoo(l);
}

 

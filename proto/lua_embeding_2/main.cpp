#include <lua.hpp>
#include <iostream>
#include <cstdint>
#include <string>
#include <cassert>

#define INFO() std::cout << "--" << __PRETTY_FUNCTION__ << std::endl;
typedef uint32_t U32;
typedef int32_t I32;
typedef int I;
typedef int8_t I8;
typedef I8 Bool8;
#define ANKI_ASSERT(x) assert(x);

lua_State* l;

template<typename... Types>
struct GetVariadicTypeId
{
	// Forward
	template<typename Type, typename... Types_>
	struct Helper;

	// Declaration
	template<typename Type, typename TFirst, typename... Types_>
	struct Helper<Type, TFirst, Types_...>: Helper<Type, Types_...>
	{};

	// Specialized
	template<typename Type, typename... Types_>
	struct Helper<Type, Type, Types_...>
	{
		static const I ID = sizeof...(Types_);
	};

	/// Get the id
	template<typename Type>
	static constexpr I get()
	{
		return sizeof...(Types) - Helper<Type, Types...>::ID - 1;
	}
};

template<typename T>
struct type_id_struct
{
    typedef T type;
    T object_instance;
};

template<int N, typename... TypeList>
struct reduce {};

template<int N, typename T1, typename... TypeList>
struct reduce<N, T1, TypeList...>
{
    typedef typename reduce<N - 1, TypeList... >::type type;
};

template<typename T1, typename... TypeList>
struct reduce<0, T1, TypeList...>
{
    typedef T1 type;
};

//convenience function
template<int N, typename... TypeList>
type_id_struct<typename reduce<N, TypeList...>::type> return_type()
{
        return type_id_struct<typename reduce<N, TypeList...>::type>();
}

/// lua userdata
struct UserData
{
	void* m_ptr = nullptr;
	Bool8 m_gc = false; ///< Garbage collection on?
};

void checkArgsCount(lua_State* l, I expected)
{
	int actualArgsCount = lua_gettop(l);
	if(expected != actualArgsCount)
	{
		luaL_error(l, "Expecting %d arguments, got %d", expected, 
			actualArgsCount);
	}
}

//==============================================================================
/// Class identification
template<typename Class>
struct ClassProxy
{
	static const char* NAME; ///< Used to check the signature of the user data

	static const char* getName()
	{
		ANKI_ASSERT(NAME != nullptr && "Class already wrapped elsewhere");
		return NAME;
	}
};

template<typename Class>
const char* ClassProxy<Class>::NAME = nullptr;

//==============================================================================
/// Used to get the function arguments from the stack
template<typename Class>
struct StackGet
{
	Class operator()(lua_State* l, I stackIndex)
	{
		UserData* udata = (UserData*)luaL_checkudata(l, stackIndex, 
			ClassProxy<Class>::getName());

		const Class* a = reinterpret_cast<const Class*>(udata->m_ptr);
		return Class(*a);
	}
};

// Specialization const ref
template<typename Class>
struct StackGet<const Class&>
{
	const Class& operator()(lua_State* l, I stackIndex)
	{
		UserData* udata = (UserData*)luaL_checkudata(l, stackIndex, 
			ClassProxy<Class>::getName());

		const Class* a = reinterpret_cast<const Class*>(udata->m_ptr);
		return *a;
	}
};

// Specialization ref
template<typename Class>
struct StackGet<Class&>
{
	Class& operator()(lua_State* l, I stackIndex)
	{
		UserData* udata = (UserData*)luaL_checkudata(l, stackIndex, 
			ClassProxy<Class>::getName());

		Class* a = reinterpret_cast<Class*>(udata->m_ptr);
		return *a;
	}
};

// Specialization const ptr
template<typename Class>
struct StackGet<const Class*>
{
	const Class* operator()(lua_State* l, I stackIndex)
	{
		UserData* udata = (UserData*)luaL_checkudata(l, stackIndex, 
			ClassProxy<Class>::getName());

		const Class* a = reinterpret_cast<const Class*>(udata->m_ptr);
		return a;
	}
};

// Specialization ptr
template<typename Class>
struct StackGet<Class*>
{
	Class* operator()(lua_State* l, I stackIndex)
	{
		UserData* udata = (UserData*)luaL_checkudata(l, stackIndex, 
			ClassProxy<Class>::getName());

		Class* a = reinterpret_cast<Class*>(udata->m_ptr);
		return a;
	}
};

#define ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(Type_, luafunc_) \
	template<> \
	struct StackGet<Type_> \
	{ \
		Type_ operator()(lua_State* l, I stackIndex) \
		{ \
			return luafunc_(l, stackIndex); \
		} \
	};

//ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(F32, luaL_checknumber)
//ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(F64, luaL_checknumber)
//ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(I8, luaL_checkinteger)
//ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(I16, luaL_checkinteger)
ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(I32, luaL_checkinteger)
//ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(I64, luaL_checkinteger)
//ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(U8, luaL_checkunsigned)
//ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(U16, luaL_checkunsigned)
ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(U32, luaL_checkunsigned)
//ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(U64, luaL_checkunsigned)
ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(const char*, luaL_checkstring)
//ANKI_STACK_GET_TEMPLATE_SPECIALIZATION(Bool, luaL_checkunsigned)

//==============================================================================
/// Call a function
template<typename T>
struct CallFunction;

// R (...)
template<typename TReturn, typename... TArgs>
struct CallFunction<TReturn (*)(TArgs...)>

	int operator()(lua_State* l, TReturn (*func)(TArgs...))
	{
		I arg = sizeof...(TArgs);
		TReturn out = (*func)(StackGet<TArgs>()(l, arg--)...);
		//PushStack<TReturn, flags> ps;
		//ps(l, out);
		return 0;
	}
};

// void (...)
template<typename... TArgs>
struct CallFunction<void (*)(TArgs...)>
{
	int operator()(lua_State* l, void (*func)(TArgs...))
	{
		(*func)(StackGet<TArgs>()(l, 1 + GetVariadicTypeId<TArgs...>::template get<TArgs>())...); 
		return 0;
	}
};

//==============================================================================
template<typename T>
struct FunctionSignature;

template<typename TReturn, typename... TArgs>
struct FunctionSignature<TReturn (*)(TArgs...)>
{
	using Func = TReturn (*)(TArgs...);
	using Return = TReturn;

	template<Func func>
	static int luafunc(lua_State* l)
	{
		checkArgsCount(l, sizeof...(TArgs));
		//auto ff = func; // A hack that saves GCC
		CallFunction<Func> cf;
		cf(l, func);
		return 0;
	}
};


int simpleFunc(lua_State*)
{
	printf("simpleFunc\n");

	return 0;
}

void simpleFunc2(int x, int y)
{
	printf("simpleFunc2 %d %d\n", x, y);
}

class Foo
{
public:
	int x = -1;

	Foo()
	{
		INFO();
	}

	Foo(int i)
		: x(i)
	{
		INFO();
	}

	Foo(const Foo& b)
		: x(b.x)
	{
		INFO();
	}

	Foo(Foo&& b)
		: x(b.x)
	{
		INFO();
	}

	~Foo()
	{
		INFO();
	}

	Foo& operator=(const Foo& b)
	{
		INFO();
		x = b.x;
		return *this;
	}

	Foo& operator=(Foo&& b)
	{
		INFO();
		x = b.x;
		return *this;
	}

	void foo()
	{
		INFO();
		std::cout << x << std::endl;
	}
};

int main(int, char**)
{
	l = luaL_newstate();
	luaL_openlibs(l);

	lua_pushcfunction(l, &simpleFunc);
	lua_setglobal(l, "simpleFunction");

	typedef FunctionSignature<decltype(&simpleFunc2)> Sig;
	lua_pushcfunction(l, &Sig::luafunc<&simpleFunc2>);
	lua_setglobal(l, "simpleFunction2");
	

	int e = luaL_dostring(l, "simpleFunction2(123.0, 456.0)");
	if(e)
	{
		std::string str(lua_tostring(l, -1));
		lua_pop(l, 1);
		std::cerr << str << std::endl;
		return 1;
	}

	return 0;
}


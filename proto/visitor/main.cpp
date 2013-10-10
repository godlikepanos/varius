#include <array>
#include <cstdio>
#include <cassert>

#define PP() printf("%s\n", __PRETTY_FUNCTION__);

/// A smart struct that given a type and a list of types finds a const integer
/// indicating the type's position from the back of the list. 
///
/// Example of usage:
/// @code
/// GetVariadicTypeId<int, float, std::string>::get<float> == 1
/// GetVariadicTypeId<int, float, std::string>::get<int> == 0
/// GetVariadicTypeId<int, float, std::string>::get<char> // Compiler error
/// @endcode
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
		static const int ID = sizeof...(Types_);
	};

	/// Get the id
	template<typename Type>
	static constexpr int get()
	{
		return sizeof...(Types) - Helper<Type, Types...>::ID - 1;
	}
};

/// A struct that from a variadic arguments list it returnes the type using an
/// ID.
/// Example of usage: 
/// @code GetTypeUsingId<double, int, float>::DataType<0> @endcode
template<typename... Types>
struct GetTypeUsingId
{
	// Forward declaration
	template<int id, typename... Types_>
	struct Helper;

	// Declaration
	template<int id, typename TFirst, typename... Types_>
	struct Helper<id, TFirst, Types_...>: Helper<id - 1, Types_...>
	{};

	// Specialized
	template<typename TFirst, typename... Types_>
	struct Helper<0, TFirst, Types_...>
	{
		typedef TFirst DataType;
	};

	template<int id>
	using DataType = typename Helper<id, Types...>::DataType;
};

/// A simple struct that creates an array of pointers to functions that have
/// the same arguments but different body
template<typename TVisitor, typename... Types>
class JumpTable
{
public:
	using FuncPtr = void (*)(TVisitor&, void*);

	JumpTable()
	{
		init<Types...>();
	}

	/// Accessor
	FuncPtr operator[](int i) const
	{
		return jumps[i];
	}

private:
	/// Pointers to JumpPoint::visit static methods
	std::array<FuncPtr, sizeof...(Types)> jumps;

	template<typename T>
	static void visit(TVisitor& v, void* address)
	{
		v.template visit(*reinterpret_cast<T*>(address));
	}

	template<typename TFirst>
	void init()
	{
		jumps[0] = &visit<TFirst>;
	}

	template<typename TFirst, typename TSecond, typename... Types_>
	void init()
	{
		constexpr int i = sizeof...(Types) - sizeof...(Types_) - 1;
		jumps[i] = &visit<TSecond>;
		init<TFirst, Types_...>();
	}
};

/// A simple struct that contains a static field with jump points
template<typename TDerived, typename... Types>
struct VisitorWrapper
{
	static const JumpTable<TDerived, Types...> jumpTable;
};

template<typename TDerived, typename... Types>
const JumpTable<TDerived, Types...> 
	VisitorWrapper<TDerived, Types...>::jumpTable;

/// Visitable class
template<typename... Types>
class Visitable
{
public:
	template<typename T>
	Visitable(T* t)
	{
		setupVisitable(t);
	}

	/// Apply visitor
	template<typename TVisitor>
	void acceptVisitor(TVisitor& v)
	{
		assert(what != -1 && address != nullptr);
		VisitorWrapper<TVisitor, Types...>::jumpTable[what](v, address);
	}

	/// Apply visitor (const version)
	template<typename TVisitor>
	void acceptVisitor(TVisitor& v) const
	{
		assert(what != -1 && address != nullptr);
		VisitorWrapper<TVisitor, Types...>::jumpTable[what](v, address);
	}

	/// Setup the data
	template<typename T>
	void setupVisitable(T* t)
	{
		assert(t != nullptr); // Null arg
		assert(address == nullptr && what == -1); // Setting for second time
		address = t;
		what = GetVariadicTypeId<Types...>::template get<T>();
	}

private:
	int what = -1; ///< The type ID
	void* address = nullptr; ///< The address to the data
};






template<typename TVisitor, typename TBase, typename... Types>
class JumpTableCommonBase
{
public:
	using FuncPtr = void (*)(TVisitor&, TBase&);

	JumpTableCommonBase()
	{
		init<Types...>();
	}

	/// Accessor
	FuncPtr operator[](int i) const
	{
		return jumps[i];
	}

private:
	/// Pointers to JumpPoint::visit static methods
	std::array<FuncPtr, sizeof...(Types)> jumps;

	template<typename T>
	static void visit(TVisitor& v, TBase& base)
	{
		v.template visit(static_cast<T&>(base));
	}

	template<typename TFirst>
	void init()
	{
		jumps[0] = &visit<TFirst>;
	}

	template<typename TFirst, typename TSecond, typename... Types_>
	void init()
	{
		constexpr int i = sizeof...(Types) - sizeof...(Types_) - 1;
		jumps[i] = &visit<TSecond>;
		init<TFirst, Types_...>();
	}
};

/// A simple struct that contains a static field with jump points
template<typename TDerived, typename TBase, typename... Types>
struct VisitorWrapperCommonBase
{
	static const JumpTableCommonBase<TDerived, TBase, Types...> jumpTable;
};

template<typename TDerived, typename TBase, typename... Types>
const JumpTableCommonBase<TDerived, TBase, Types...> 
	VisitorWrapperCommonBase<TDerived, TBase, Types...>::jumpTable;


/// XXX
template<typename TBase, typename... Types>
class VisitableCommonBase
{
public:

#if ~defined(NDEBUG)
	// Allow dynamic cast in acceptVisitor
	virtual ~VisitableCommonBase()
	{}
#endif

	/// Apply visitor
	template<typename TVisitor>
	void acceptVisitor(TVisitor& v)
	{
		assert(what != -1);
#if defined(NDEBUG)
		TBase* base = static_cast<TBase*>(this);
#else
		TBase* base = dynamic_cast<TBase*>(this);
		assert(base != nullptr);
#endif 
		VisitorWrapperCommonBase<TVisitor, TBase, Types...>::
			jumpTable[what](v, *base);
	}

	/// Apply visitor (const version)
	template<typename TVisitor>
	void acceptVisitor(TVisitor& v) const
	{
		typedef const TBase CTBase;
		assert(what != -1);
#if defined(NDEBUG)
		CTBase* base = static_cast<CTBase*>(this);
#else
		CTBase* base = dynamic_cast<CTBase*>(this);
		assert(base != nullptr);
#endif 
		VisitorWrapperCommonBase<TVisitor, CTBase, const Types...>::
			jumpTable[what](v, *base);
	}

	/// Setup the data
	template<typename T>
	void setupVisitable(const T*)
	{
		assert(what == -1); // Setting for second time
		what = GetVariadicTypeId<Types...>::template get<T>();
	}

private:
	int what = -1; ///< The type ID
};


struct Dcs;
struct Ics;
struct Scs;

struct Foo
{
	int x = 123;
	int y = 456;
};

/*struct Cs: Visitable<Ics, Dcs, Scs>
{
	template<typename T>
	Cs(T* t)
		: Visitable<Ics, Dcs, Scs>(t)
	{}
};

struct Dcs: Cs
{
	Dcs()
		: Cs(this)
	{
		//setupVisitable(this);
	}

	int z = 777;
};


struct Ics: Foo, Cs
{
	Ics()
		: Cs(this)
	{
		//setupVisitable(this);
	}

	int z = 888;
};

struct Scs: Cs
{
	Scs()
		: Cs(this)
	{
		//setupVisitable(this);
	}

	int z = 999;
};*/

struct Cs: VisitableCommonBase<Cs, Ics, Dcs, Scs>
{
	int x = 111;
};

struct Dcs: Cs
{

	int z = 777;

	Dcs()
	{
		setupVisitable(this);
	}
};


struct Ics: Foo, Cs
{
	int z = 888;

	Ics()
	{
		setupVisitable(this);
	}
};

struct Scs: Cs
{
	int z = 999;

	Scs()
	{
		setupVisitable(this);
	}
};


struct MVisitor
{
	template<typename T>
	void visit(T& x)
	{
		PP()
		printf("&?: %p\n", &x);
	}
};

struct CVisitor
{
	template<typename T>
	void visit(const T& x)
	{
		PP()
		printf("&?: %p\n", &x);
	}
};


int main(int, char**) 
{
	Dcs d;
	Ics i;
	Scs s;
	Cs* cs;
	const Cs* ccs;
	MVisitor vis;
	CVisitor bvis;

	printf("&d: %p\n", &d);
	printf("&i: %p\n", &i);
	printf("&s: %p\n", &s);

	cs = &d;
	cs->acceptVisitor(vis);
	cs = &i;
	cs->acceptVisitor(vis);
	cs = &s;
	cs->acceptVisitor(vis);

	ccs = &d;
	ccs->acceptVisitor(bvis);

	return 0;
}


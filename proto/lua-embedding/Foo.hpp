#ifndef FOO_HPP
#define FOO_HPP

#include <iostream>

#define IN(x) std::cout << __PRETTY_FUNCTION__ << ":" << __LINE__ \
	<< "\t" << x << std::endl

#define INFOO() IN("id:" << id << ", x:" << x << ", y:" << y)

class Foo
{
public:
	Foo()
		: x(1.0), y(2.0)
	{
		id = gid++;
		INFOO();
	}

	Foo(float f)
		: x(f), y(f)
	{
		id = gid++;
		INFOO();
	}

	Foo(float x_, float y_)
		: x(x_), y(y_)

	{
		id = gid++;
		INFOO();
	}

	Foo(const Foo& b)
		: x(b.x), y(b.y)
	{
		id = gid++;
		INFOO();
	}

	~Foo()
	{
		INFOO();
	}

	Foo& operator=(const Foo& b)
	{
		INFOO();
		x = b.x;
		y = b.y;
		return *this;
	}

	int methodii(int i)
	{
		INFOO();
		std::cout << i << std::endl;
		return i + 1;
	}

	void methodargfoo(Foo b)
	{
		INFOO();
	}

	void methodvoid()
	{
		INFOO();
	}

	Foo& methodfoo()
	{
		INFOO();
		return *this;
	}

	Foo* methodptr()
	{
		INFOO();
		return this;
	}

	const char* methodstr(const char* x)
	{
		static std::string ss{x + std::string("_kolos")};
		INFOO();
		return ss.c_str();
	}

	static const Foo& getZero()
	{
		static const Foo zero(0.0);
		IN("");
		return zero;
	}

	virtual void virt()
	{
		INFOO();
	}

	static void setZero(Foo& a)
	{
		IN(" ");
		a = Foo(0.0);
	}

	float x;
	float y;
	int id;
	static int gid;
};

#endif


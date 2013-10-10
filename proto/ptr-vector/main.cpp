#include <iostream>
#include <memory>
#include <vector>

#define WHERE() std::cout << __PRETTY_FUNCTION__ << std::endl

struct Foo
{
	Foo()
	{
		x = s++;
		WHERE();
	}

	Foo(const Foo&)
	{
		x = s++;
		WHERE();
	}

	~Foo()
	{
		WHERE();
	}

	Foo& operator=(const Foo&)
	{
		WHERE();
		return *this;
	}

	static int s;
	int x;
};

int Foo::s = 0;

typedef std::vector<std::shared_ptr<Foo>> FooVec;

int main(int, char**)
{
	FooVec vec, vec2;

	vec.push_back(std::shared_ptr<Foo>(new Foo()));
	vec.push_back(std::shared_ptr<Foo>(new Foo()));

	std::cout << "Before swap" << std::endl;
	//vec.swap(vec2);
	std::cout << "After swap" << std::endl;

	std::cout << vec[1]->x << std::endl;

	return 0;
}

#include <iostream>
#include <thread>

template<typename T>
class SingletonThreadSafe
{
public:
	typedef T Value;

	// Non copyable
	SingletonThreadSafe(const SingletonThreadSafe&) = delete;
	SingletonThreadSafe& operator=(const SingletonThreadSafe&) = delete;

	// Non constructable
	SingletonThreadSafe() = delete;
	~SingletonThreadSafe() = delete;

	/// Get instance
	static Value& get()
	{
		return *(instance ? instance : (instance = new Value));
	}

private:
	static __thread Value* instance;
};

template <typename T>
__thread typename SingletonThreadSafe<T>::Value* 
	SingletonThreadSafe<T>::instance = 0;

class Foo
{
public:
	Foo()
	{
		std::cout << "I am " << std::this_thread::get_id() 
			<< " and " << __PRETTY_FUNCTION__ << std::endl;
	}

	static __thread int fooska;
};

int __thread Foo::fooska = 123;

typedef SingletonThreadSafe<Foo> FooSingleton;

struct Worker
{
	void operator()()
	{
		FooSingleton::get();

		for(int i = 0; i < 1000000000; ++i)
		{
			++Foo::fooska;
		}

		std::cout << "I am " << std::this_thread::get_id() 
			<< " and " << Foo::fooska << std::endl;
	}
};

int main(int, char**) 
{
	std::cout << "main() starts" << std::endl;

	std::thread thr0, thr1;

	thr0 = std::thread(Worker());
	thr1 = std::thread(Worker());

	thr0.join();
	thr1.join();

	return 0;
}

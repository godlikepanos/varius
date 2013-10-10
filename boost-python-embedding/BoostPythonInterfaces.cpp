#include <vector>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include "Vec3.h"
#include "Vec3.h"
#include "ReturnReference.h"
#include "StaticCast.h"
#include "Util/Vec.h"


using namespace boost::python;
using namespace boost;

void setInt(int& self, int value) {
			self = value;
	}

int getInt(ReturnReference* r)
{
	return r->getInt();
}



class A
{
	public:
		template<typename Type>
		class Types
		{
			public:
				typedef Vec<Type*> Container;
				typedef typename Container::iterator Iterator;
				typedef typename Container::const_iterator ConstIterator;
		};

		const Types<int>::Container& getInts() const {return ints;}
		Types<int>::Container& getInts() {return ints;}

		Types<int>::Container ints;
};


typedef A::Types<int>::Container lll;

BOOST_PYTHON_MODULE(Anki)
{
	class_<A>("A")
		.def("getInts", (A::Types<int>::Container& (A::*)())(&A::getInts), return_value_policy<reference_existing_object>())
	;

	class_<Vec3>("Vec3")
		.def_readwrite("x", &Vec3::x)
		.def_readwrite("y", &Vec3::y)
		.def_readwrite("z", &Vec3::z)
		.def(init<>())
		.def(init<const Vec3&>())
		.def(init<float, float, float>())
		.def(init<float>())
		.def(init<const Vec4&>())
		.def("getStr", (const std::string& (Vec3::*)() const)(&Vec3::getStr), return_value_policy<copy_const_reference>())
	;


	class_<Vec4>("Vec4")
		.def_readwrite("x", &Vec4::x)
		.def_readwrite("y", &Vec4::y)
		.def_readwrite("z", &Vec4::z)
		.def_readwrite("w", &Vec4::w)
		.def(init<const Vec4&>())
		.def(init<float, float, float, float>())
		.def(init<float>())
		.def(init<const Vec3&, float>())
		.def("getInstance", &Vec4::getInstance, return_value_policy<reference_existing_object>())
		.staticmethod("getInstance")
	;


	/*class_<int>("int")
		.def("set", &setInt)
	;*/


	class_<ClassA>("ClassA")
		.def_readwrite("a", &ClassA::a)
		.def("upClass", &ClassA::upClass, return_value_policy<reference_existing_object>())
	;


	class_<ClassB, bases<ClassA> >("ClassB")
		.def_readwrite("b", &ClassB::b)
	;
}


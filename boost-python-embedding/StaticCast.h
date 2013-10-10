#ifndef STATIC_CAST_H
#define STATIC_CAST_H


class ClassB;

class ClassA
{
	public:
		int a;
		ClassA(): a(666) {}

		ClassB& upClass();
};


class ClassB: public ClassA
{
	public:
		int b;
		ClassB(): b(777) {}
};


inline ClassB& ClassA::upClass()
{
	return static_cast<ClassB&>(*this);
}


#endif

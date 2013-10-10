#ifndef RETURN_REFERENCE_H
#define RETURN_REFERENCE_H

#include <iostream>


/// Read write property
///
/// - It deliberately does not work with pointers
/// - The get funcs are coming into two flavors, one const and one non-const. The property is read-write after all so
///   the non-const is acceptable
/// - Dont use it with semicolon at the end (eg PROPERTY_RW(....);) because of a doxygen bug
#define PROPERTY_RW(Type__, varName__, setFunc__, getFunc__) \
	private: \
		Type__ varName__; \
	public: \
		void setFunc__(const Type__& x__) {varName__ = x__;} \
		void setFunc__##Value(Type__ x__) {varName__ = x__;} \
		const Type__& getFunc__() const {return varName__;} \
		Type__& getFunc__() {return varName__;} \
		Type__ getFunc__ ## Value() const {return varName__;}


/// Read only private property
///
/// - It deliberately does not work with pointers
/// - Dont use it with semicolon at the end (eg PROPERTY_RW(....);) because of a doxygen bug
#define PROPERTY_R(Type__, varName__, getFunc__) \
	private: \
		Type__ varName__; \
	public: \
		const Type__& getFunc__() const {return varName__;}


using namespace std;


class ReturnReference
{
	PROPERTY_RW(int, i, setInt, getInt)

	public:
		ReturnReference(): i(123) {}
};


#endif

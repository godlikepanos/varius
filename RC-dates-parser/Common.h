#ifndef COMMON_H
#define COMMON_H

namespace std
{}

using namespace std;

#ifndef ulong
typedef unsigned long int ulong;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

#define ERROR(x) cerr << __FILE__ << ":" << __LINE__ << " " << x << endl;

#define PRINT(x) cout << x << endl;

#define DEBUG_ERR(x) \
	if(x) \
		ERROR(#x)

/**
 * Read write property
 *
 * - It creates a unique type so it can work with pointers
 * - The get funcs are coming into two flavors, one const and one non-const. The property is read-write after all so the
 *   non-const is acceptable
 * - Dont use it with semicolon at the end (eg PROPERTY_RW(....);) because of a doxygen bug
 */
#define PROPERTY_RW(__Type__, __varName__, __setFunc__, __getFunc__) \
	protected: \
		typedef __Type__ __Dummy__##__varName__; \
		__Dummy__##__varName__ __varName__; \
	public: \
		void __setFunc__(const __Dummy__##__varName__& __x__) { \
			__varName__ = __x__; \
		} \
		const __Dummy__##__varName__& __getFunc__() const { \
			return __varName__; \
		} \
		__Dummy__##__varName__& __getFunc__() { \
			return __varName__; \
		}

/**
 * Read only property
 *
 * - It creates a unique type so it can work with pointers
 * - Dont use it with semicolon at the end (eg PROPERTY_RW(....);) because of a doxygen bug
 */
#define PROPERTY_R(__Type__, __varName__, __getFunc__) \
	protected: \
		typedef __Type__ __Dummy__##__varName__; \
		__Dummy__##__varName__ __varName__; \
	public: \
		const __Dummy__##__varName__& __getFunc__() const { \
			return __varName__; \
		}


#endif

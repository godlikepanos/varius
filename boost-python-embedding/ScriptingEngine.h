#ifndef SCRIPTIN_GENGINE_H
#define SCRIPTIN_GENGINE_H

#include <boost/python.hpp>


using namespace boost::python;


/**
 * The scripting engine
 */
class ScriptingEngine
{
	public:
		/**
		 * Init python and modules
		 */
		bool init();

		/**
		 * Execute python script
		 * @param script Script source
		 * @return true on success
		 */
		bool execScript(const char* script);

		/**
		 * Expose a C++ variable to python
		 * @param varName The name to referenced in python
		 * @param var The actual variable
		 */
		template<typename Type>
		void exposeVar(const char* varName, Type* var);

	private:
		boost::python::object mainModule;
		boost::python::object ankiModule;
		boost::python::object mainNamespace;
};


template<typename Type>
void ScriptingEngine::exposeVar(const char* varName, Type* var)
{
	scope(ankiModule).attr(varName) = ptr(var);
}


#endif

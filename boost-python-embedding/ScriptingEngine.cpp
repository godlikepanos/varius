#include <Python.h>
#include <iostream>
#include "ScriptingEngine.h"


extern "C" void initAnki(); /// Defined in BoostPythonInterfaces.cpp by boost

using namespace boost::python;
using namespace std;


//==============================================================================
// init                                                                        =
//==============================================================================
bool ScriptingEngine::init()
{
	PyImport_AppendInittab((char*)("Anki"), &initAnki);
	Py_Initialize();
	mainModule = object(handle<>(borrowed(PyImport_AddModule("__main__"))));
	mainNamespace = mainModule.attr("__dict__");
	ankiModule = object(handle<>(PyImport_ImportModule("Anki")));

	return true;
}


//==============================================================================
// execScript                                                                  =
//==============================================================================
bool ScriptingEngine::execScript(const char* script)
{
	try
	{
		handle<>ignored(PyRun_String(script, Py_file_input, mainNamespace.ptr(), mainNamespace.ptr()));
	}
	catch(error_already_set)
	{
		PyErr_Print();
		return false;
	}
	return true;
}

#include <Python.h>
#include <boost/python.hpp>
#include <iostream>
#include <fstream>
#include "Vec3.h"
#include "Vec4.h"
#include "ScriptingEngine.h"
#include "StaticCast.h"
#include "Classes.h"


using namespace boost;
using namespace std;


int main(int /*argc*/, char** /*argv*/)
{
	ScriptingEngine se;
	se.init();

	Vec3* a = new Vec3();
	a->getStr() = "skatoules";

	A a;

	se.exposeVar("a", &a);

	ifstream file("test.py");
	if(!file.is_open())
	{
		cerr << "Cannot open file" << endl;
		return 1;
	}
	string source((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	se.execScript(source.c_str());

	//se.execScript("from Anki import *\nprint(Vec4.getInstance().x)");

	return 0;
}

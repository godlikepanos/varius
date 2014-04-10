#include <iostream>
#include "MyApplication.h"

Application* app;


//======================================================================================================================
// main                                                                                                                =
//======================================================================================================================
int main(int /*argc*/, char** /*argv*/)
{
	cout << "Starting..." << endl;

	MyApplication* myApp = new MyApplication(1000, 1000);
	app = myApp;
	
	myApp->initAdditionalPhysics();

	return app->exec();
}

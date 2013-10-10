#include <QCoreApplication>
#include <QtDebug>
#include <QScriptEngine>
#include <QStringList>
#include "MyObject.h"


QCoreApplication* app;
MyObject* obj;
QScriptEngine* scrEnging;

//QString prog = "print(obj.getObj2())";
QString prog =
"var out = []\n" \
"for (attr in obj)\n"\
"{ out.push(attr); }\n" \
"print(out)\n" \
"print(obj.getObj2())";


Q_DECLARE_METATYPE(MyObject2*)


QScriptValue myObjectToScriptValue(QScriptEngine* engine, MyObject2* const & in)
{
	return engine->newQObject(in);
}

void myObjectFromScriptValue(const QScriptValue& object, MyObject2* &out)
{
	out = qobject_cast<MyObject2*> (object.toQObject());
}


int main(int argc, char** argv)
{
	app = new QCoreApplication(argc, argv);
	obj = new MyObject(app);
	scrEnging = new QScriptEngine(app);

	qScriptRegisterMetaType(scrEnging, myObjectToScriptValue, myObjectFromScriptValue);


	QScriptValue objVal = scrEnging->newQObject(obj);
	scrEnging->globalObject().setProperty("obj", objVal);
	scrEnging->evaluate(prog, "TestProg");
	if(scrEnging->hasUncaughtException())
	{
		QStringList list = scrEnging->uncaughtExceptionBacktrace();
		for(int i=0; i<list.size(); i++)
			cout << qPrintable(list[i]) << endl;
	}

	//cout << qPrintable(obj->) << endl;

	delete app;
	return 0;
}

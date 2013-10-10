#ifndef MY_OBJECT_H
#define MY_OBJECT_H

#include <QObject>
#include <iostream>
#include <QScriptable>
#include "MyObject2.h"


using namespace std;


/**
 * Dummy QObject
 */
class MyObject: public QObject, public QScriptable
{
	Q_OBJECT

	//Q_PROPERTY(MyObject2 *obj2 READ getObj2 SCRIPTABLE true)

	public:

		MyObject(QObject* parent = NULL);
		~MyObject();

	public slots:
		MyObject2& getObj2() {return *obj2;}

	public:
		Q_INVOKABLE MyObject2* obj2;
};


inline MyObject::MyObject(QObject* parent):
	QObject(parent),
	obj2(new MyObject2(this))
{
	cout << "Constructor" << endl;
}


inline MyObject::~MyObject()
{
	cout << "Destructor" << endl;
}


#endif

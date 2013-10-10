#ifndef MY_OBJECT2_H
#define MY_OBJECT2_H

#include <QObject>
#include <iostream>
#include <QScriptable>


using namespace std;


/**
 * Dummy QObject
 */
class MyObject2: public QObject, public QScriptable
{
	Q_OBJECT

	Q_PROPERTY(QString str READ getStr SCRIPTABLE true)

	public:
		MyObject2(QObject* parent = NULL);
		~MyObject2();
		const QString& getStr() const {return str;}

	private:
		QString str;
};


inline MyObject2::MyObject2(QObject* parent):
	QObject(parent),
	str("lalala")
{
	cout << "Constructor" << endl;
}


inline MyObject2::~MyObject2()
{
	cout << "Destructor" << endl;
}


#endif

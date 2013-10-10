#include <iostream>
#include <vector>
#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <poll.h>
#include <string>
#include <cstdio>
#include "StdinListener.h"


using namespace std;
using namespace boost;





/*
static void pullFromStd()
{
	char buff[256];

	while(1)
	{
		int m = read(0, buff, sizeof(buff));
		buff[m] = '\0';
		//cout << "read: " << buff << endl;
		{
			mutex::scoped_lock lock(ioMutex);
			q.push(buff);
			cout << "size:" << q.size() << endl;
		}
	}
}


string getLine()
{
	string ret;
	mutex::scoped_lock lock(ioMutex);
	cout << "_size:" << q.size() << endl;
	if(!q.empty())
	{
		ret = q.front();
		q.pop();
	}
	return ret;
}
*/


int main(int /*argc*/, char* /*argv*/[])
{
	StdinListener listener;
	thread thr(&StdinListener::operator(), &listener);

	for(;;)
	{
		string str = listener.getLine();
		if(str.length())
			cout << "have line" << endl;
		//cout << "Main loop" << endl;
		sleep(1);
	}

	return 0;
}


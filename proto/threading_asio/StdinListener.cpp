#include "StdinListener.h"


//======================================================================================================================
// operator()                                                                                                          =
//======================================================================================================================
void StdinListener::operator()()
{
	char buff[512];

	while(1)
	{
		int m = read(0, buff, sizeof(buff));
		buff[m] = '\0';
		//cout << "read: " << buff << endl;
		{
			mutex::scoped_lock lock(mtx);
			q.push(buff);
			//cout << "size:" << q.size() << endl;
		}
	}
}


//======================================================================================================================
// getLine                                                                                                             =
//======================================================================================================================
string StdinListener::getLine()
{
	string ret;
	mutex::scoped_lock lock(mtx);
	//cout << "_size:" << q.size() << endl;
	if(!q.empty())
	{
		ret = q.front();
		q.pop();
	}
	return ret;
}

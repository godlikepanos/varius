#ifndef STDIN_LISTENER_H
#define STDIN_LISTENER_H

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <queue>


using namespace std;
using namespace boost;


/**
 * The listener of the stdin
 */
class StdinListener
{
	public:
		StdinListener() {}
		~StdinListener() {}
		void operator()(); ///< The thread function
		string getLine();

	private:
		queue<string> q;
		mutex mtx;

		StdinListener(const StdinListener&) {} ///< Non copyable
};


#endif

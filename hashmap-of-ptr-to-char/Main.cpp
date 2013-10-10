#include <iostream>
#include <boost/unordered_map.hpp>
#include <cstring>


using namespace std;
using namespace boost;


struct CreateCharPtrHashMapKey
{
  size_t operator()(const char* str) const
	{
		size_t h = 0;
		size_t len = 0;
		for (; *str != '\0'; ++str)
		{
			h += *str;
			++len;
		}
		return h + len;
	}
};


struct CompareCharPtrHashMapKeys
{
  bool operator()(const char* /*s1*/, const char* /*s2*/) const
  {
    return true;
  }
};


//typedef unordered_map<const char*, int, CreateCharPtrHashMapKey,CompareCharPtrHashMapKeys> CharPtrHashMap;
template<typename Type>
class CharPtrHashMap: public unordered_map<const char*, Type, CreateCharPtrHashMapKey,CompareCharPtrHashMapKeys>
{};



int main(int /*argc*/, char* /*argv*/[])
{
	CharPtrHashMap<int> mm;
	mm["apple"] = 1;
	mm["orange"] = 2;
	string zzz("purple");
	const char *xxx = zzz.c_str();
	mm[xxx] = 3;

	char arr[] = {'p', 'u', 'r', 'p', 'l', 'e', '\0', '_'};

	//const_cast<char*> (xxx)[1] = '9';

	CharPtrHashMap<int>::iterator it_;
	for (it_ = mm.begin(); it_ != mm.end(); it_++)
	{
		cout << it_->first << endl;
	}

	CharPtrHashMap<int>::iterator it = mm.find(arr);
	cout << it->second << endl;

	return 0;
}

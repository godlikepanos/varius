#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "z/contrib/minizip/unzip.h"

int main(int, char**)
{
	unzFile zfile = unzOpen("../archive.zip");

	if(zfile == NULL)
	{
		std::cerr << "Cannot open zip file" << std::endl;
		return 1;
	}

	if(UNZ_OK != unzLocateFile(zfile, "archive/haha.txt", 1))
	{
		std::cerr << "unzLocateFile failed" << std::endl;
		return 1;
	}

	if(UNZ_OK != unzOpenCurrentFile(zfile))
	{
		std::cerr << "unzOpenCurrentFile failed" << std::endl;
		return 1;
	}

	char buff[256];
	int count = unzReadCurrentFile(zfile, buff, sizeof(buff));
	if(count < 0)
	{
		std::cerr << "unzReadCurrentFile failed" << std::endl;
		return 1;
	}

	std::cout << buff << std::endl;

	unzClose(zfile);

	return 0;
}

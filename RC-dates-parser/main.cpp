#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;

#include "Common.h"
#include "Parser.h"


int main(int argc, char* argv[])
{
	Scanner scanner;
	const Scanner::Token* token;

	scanner.loadFile("/home/godlike/tmp/rc.txt");

	while(true)
	{
		int day;
		int month;
		int year;
		string venue;
		string city;
		string country;

		// day
		token = &scanner.getNextToken();
		if(token->getCode() != Scanner::TC_NUMBER || token->getDataType() != Scanner::DT_INT)
		{
			PARSE_ERR_EXPECTED("integer");
			return 1;
		}
		day = token->getValue().getInt();

		// month
		token = &scanner.getNextToken();
		if(token->getCode() != Scanner::TC_IDENTIFIER)
		{
			PARSE_ERR_EXPECTED("identifier");
			return 1;
		}

		if(string("Jan") == token->getValue().getString())
		{
			month = 1;
		}
		else if(string("Feb") == token->getValue().getString())
		{
			month = 2;
		}
		else if(string("Mar") == token->getValue().getString())
		{
			month = 3;
		}
		else if(string("Apr") == token->getValue().getString())
		{
			month = 4;
		}
		else if(string("May") == token->getValue().getString())
		{
			month = 5;
		}
		else if(string("Jun") == token->getValue().getString())
		{
			month = 6;
		}
		else if(string("Jul") == token->getValue().getString())
		{
			month = 7;
		}
		else if(string("Aug") == token->getValue().getString())
		{
			month = 8;
		}
		else if(string("Sep") == token->getValue().getString())
		{
			month = 9;
		}
		else if(string("Oct") == token->getValue().getString())
		{
			month = 10;
		}
		else if(string("Nov") == token->getValue().getString())
		{
			month = 11;
		}
		else if(string("Dec") == token->getValue().getString())
		{
			month = 12;
		}
		else
		{
			PARSE_ERR("See file");
		}

		// year
		token = &scanner.getNextToken();
		if(token->getCode() != Scanner::TC_NUMBER || token->getDataType() != Scanner::DT_INT)
		{
			PARSE_ERR_EXPECTED("integer");
			return 1;
		}
		year = token->getValue().getInt();

		// time
		/*token = &scanner.getNextToken();
		if(token->getCode() != Scanner::TC_PERIOD)
		{
			PARSE_ERR_EXPECTED("-");
			return 1;
		}

		token = &scanner.getNextToken();
		if(token->getCode() != Scanner::TC_NUMBER || token->getDataType() != Scanner::DT_INT)
		{
			PARSE_ERR_EXPECTED("integer");
			return 1;
		}

		token = &scanner.getNextToken();
		if(token->getCode() != Scanner::TC_UPDOWNDOT)
		{
			PARSE_ERR_EXPECTED(":");
			return 1;
		}

		token = &scanner.getNextToken();
		if(token->getCode() != Scanner::TC_NUMBER || token->getDataType() != Scanner::DT_INT)
		{
			PARSE_ERR_EXPECTED("integer");
			return 1;
		}*/

		// venue
		token = &scanner.getNextToken();
		if(token->getCode() != Scanner::TC_PERIOD)
		{
			PARSE_ERR_EXPECTED(";");
			return 1;
		}

		while(true)
		{
			token = &scanner.getNextToken();
			if(token->getCode() == Scanner::TC_PERIOD)
				break;

			venue += token->getString();
			venue += " ";
		}

		// city
		while(true)
		{
			token = &scanner.getNextToken();

			if(token->getCode() == Scanner::TC_PERIOD)
				break;

			city += token->getString();
			city += " ";
		}

		// country
		token = &scanner.getNextToken();
		if(token->getCode() != Scanner::TC_IDENTIFIER && token->getCode() != Scanner::TC_STRING)
		{
			PARSE_ERR_EXPECTED("identifier or string");
			return 1;
		}
		country = token->getValue().getString();

		// end
		/*while(true)
		{
			token = &scanner.getNextToken();
			if(token->getCode() == Scanner::TC_PERIOD)
			{
				break;
			}
		}*/

		// print
		PRINT("INSERT INTO tours VALUES ( NULL , '" << year << '-' << month << '-' << day << "', '" << country << "', '"
		    << city << "', '" << venue << "', '123' );");
	}

	return 0;
}

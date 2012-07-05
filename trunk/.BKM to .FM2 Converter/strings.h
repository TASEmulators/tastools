/*
 .BKM to .FM2 Converter - Strings Library
 Just some custom string functions used in the program.
 By Brandon Evans - http://www.brandonevans.org/
*/

#ifndef _STRINGS_H
#define _STRINGS_H

#include <string>

using namespace std;

string lower(string str);

string upper(string str);

string trim(const string &str);

string findAndReplace(const string &find, const string &replace, string str);

bool startsWith(string str, const string &line);

#endif
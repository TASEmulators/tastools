#include <algorithm>
#include "strings.h"

string lower(string str)
{
	// Convert to lowercase.
	transform(str.begin(), str.end(), str.begin(), tolower);
	return str;
}

string upper(string str)
{
	// Convert to uppercase.
	transform(str.begin(), str.end(), str.begin(), toupper);
	return str;
}

string trim(const string &str)
{
	// Remove leading / trailing whitespace.
    unsigned int start = str.find_first_not_of(" \n\r\t");
    unsigned int end = str.find_last_not_of (" \n\r\t");
    if ((start == string::npos) || (end == string::npos))
        return "";
    else
        return str.substr(start, end - start + 1);
}

string findAndReplace(const string &find, const string &replace, string str)
{
	unsigned int pos = 0;
	while((pos = str.find(find, pos)) != string::npos)
	{
		str.replace(pos, find.length(), replace);
		pos += replace.length();
	}
	return str;
}

bool startsWith(string str, const string &line)
{
	return (lower(line).find(lower(str)) == 0);
}
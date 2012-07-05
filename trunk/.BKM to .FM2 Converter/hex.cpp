/*
 Hex <=> String Converter
 Converts hexadecimal byte strings to strings and vice-versa.
 By FredOverflow - http://stackoverflow.com/users/252000/fredoverflow
 With modifications by Brandon Evans - http://www.brandonevans.org/
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "hex.h"
#include "strings.h"

std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();

    std::string output;
    output.reserve(2 * len);
    for (size_t i = 0; i < len; ++i)
    {
        const char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}

#include <algorithm>
#include <stdexcept>

std::string hex_to_string(std::string input)
{
	input = upper(input);
    static const char* const lut = "0123456789ABCDEF";
    size_t len = input.length();
    if (len & 1)
	{
		std::cout << "Warning: Hex string can't have an odd length." << endl;
		return "";
	}

    std::string output;
    output.reserve(len / 2);
    for (size_t i = 0; i < len; i += 2)
    {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);
        if (*p != a)
		{
			std::cout << "Warning: Hex string contains a non-hex digit." << endl;
			return "";
		}

        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);
        if (*q != b)
		{
			std::cout << "Warning: Hex string contains a non-hex digit." << endl;
			return "";
		}

        output.push_back(((p - lut) << 4) | (q - lut));
    }
    return output;
}
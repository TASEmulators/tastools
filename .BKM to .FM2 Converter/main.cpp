/*
 .BKM to .FM2 Converter
 Converts a movie playback file for BizHawk to one for FCEUX.
 By Brandon Evans - http://www.brandonevans.org/
*/

#include <iostream>
#include <fstream>
#include <string>
#include "strings.h"
#include "hex.h"
#include "base64.h"

using namespace std;

void heading()
{
	cout << "------------------------------------------------------------" << endl;
	cout << ".BKM to .FM2 Converter" << endl;
	cout << "Converts a movie playback file for BizHawk to one for FCEUX." << endl;
	cout << "By Brandon Evans - http://www.brandonevans.org/" << endl;
	cout << "------------------------------------------------------------" << endl << endl;
}

string inputFilename(int argc, char *argv[])
{
	string input;
	// If there are no arguments, prompt the user for the input filename.
	if (argc < 2)
	{
		cout << "Input file: ";
		cin >> input;
	}
	else
	{
		// Otherwise, it's the first command-line argument.
		input = argv[1];
		cout << "Input file: " << input << endl;
	}
	return input;
}

string outputFilename(int argc, char *argv[], string input)
{
	string output;
	// If there are no arguments, prompt the user for the output filename.
	if (argc < 2)
	{
		cout << "Output file: ";
		cin >> output;
	}
	else
	{
		// If only the input filename is provided, derive the output filename from it.
		if (2 == argc)
		{
			output = input + ".fm2";
		}
		else
		{
			// Otherwise, it's the second command-line argument.
			output = argv[2];
		}
		cout << "Output file: " << output << endl;
	}
	return output;
}

string header(string name, const string &line)
{
	int pos;
	// Case-insensitive search.
	pos = lower(line).find(lower(name));
	// Get the contents for this header.
	return trim(line.substr(pos + name.length()));
}

void frame(const string &line, ofstream &out)
{
	unsigned int pos;
	// UDLRsSBA to RLDUTSBA
	int buttons[8] = {3, 2, 1, 0, 5, 4, 6, 7};
	string mnemonic = "RLDUTSBA";
	out << "|";
	// Convert the flag.
	if ('r' == line[1])
		out << "1";
	else
		out << "0";
	out << "|";
	pos = 3;
	for (int controller = 0; controller < 2; controller++)
	{
		// Only read a controller if there are 8 buttons.
		if (line.length() < pos + 8)
			break;
		for (int button = 0; button < 8; button++)
		{
			// Find this button's state and mark accordingly.
			if (line[pos + buttons[button]] != '.')
				out << mnemonic[button];
			else
				out << '.';
		}
		out << "|";
		pos += 9;
	}
	out << "|";
}

void subtitle(string line, ofstream &out)
{
	int start;
	int end;
	string prev;
	out << "subtitle";
	// Reduce all whitespace to single spaces.
	line = findAndReplace("\t", " ", line);
	line = findAndReplace("\n", " ", line);
	line = findAndReplace("\r", " ", line);
	line = findAndReplace("\r\n", " ", line);
	do
	{
		prev = line;
		line = findAndReplace("  ", " ", line);
	}
	while (prev != line);
	// header frame x y length color message
	start = line.find(" ");
	end = line.find(" ", start + 1);
	if (start != string::npos && end != string::npos)
	{
		// Grab the frame number.
		out << " " << line.substr(start + 1, end - start - 1);
		// Start now points to space before y.
		start = line.find(" ", end + 1);
		// length
		start = line.find(" ", start + 1);
		// color
		start = line.find(" ", start + 1);
		// message
		start = line.find(" ", start + 1);
		if (start != string::npos)
			// Grab the rest for the message.
			out << " " << trim(line.substr(start + 1));
	}
}

void convert(ifstream &in, ofstream &out)
{
	string line;
	if (!in.is_open())
	{
		cout << "Error: Can't open input file." << endl;
		return;
	}
	if (!out.is_open())
	{
		cout << "Error: Can't open output file." << endl;
		return;
	}
	out << "version 3" << endl;
	// Read each line.
	while (!in.eof())
	{
		line.clear();
		getline(in, line);
		if (line == "")
			continue;
		if (line.length() > 1 && '|' == line[0])
			frame(line, out);
		else if (startsWith("sub", line))
		{
			subtitle(line, out);
		}
		else if (startsWith("emuOrigin FCEUX version", line))
			out << "emuVersion " << header("emuOrigin FCEUX version", line);
		else if (startsWith("GameName", line))
			out << "romFilename " << header("GameName", line);
		else if (startsWith("MD5", line))
		{
			string checksum = hex_to_string(header("MD5", line));
			checksum = base64_encode(reinterpret_cast<const unsigned char*>(checksum.c_str()), checksum.length());
			out << "romChecksum base64:" << checksum;
		}
		else if (startsWith("Author", line))
			out << "comment author " << header("Author", line);
		else if (startsWith("rerecordCount", line))
			out << "rerecordCount " << header("rerecordCount", line);
		else if (startsWith("GUID", line))
			out << "guid " << header("GUID", line);
		else if (startsWith("StartsFromSavestate", line))
		{
			cout << "Movies that begin with a savestate are not supported.";
			return;
		}
		else if (startsWith("PAL", line))
			out << "palFlag " << ((header("PAL", line) == "True") ? 1 : 0);
		else if (startsWith("FourScore", line))
			out << "fourscore " << ((header("FourScore", line) == "True") ? 1 : 0);
		else if (startsWith("MovieVersion", line))
		{
			if (header("MovieVersion", line) != "BizHawk v0.0.1")
			{
				cout << "Error: .BKM movie version must always be v0.0.1" << endl;
				return;
			}
			continue;
		}
		else if (
			startsWith("emuVersion", line) || startsWith("Platform", line) ||
			startsWith("SHA1", line) || startsWith("emuOrigin", line) ||
			startsWith("MovieOrigin", line) || startsWith("SyncHack", line)
		)
			continue;
		else
			out << line;
		out << endl;
	}
	cout << "Output file written successfully." << endl;
}

int main(int argc, char *argv[])
{
	string input, output;
	ifstream in;
	ofstream out;
	heading();
	input = inputFilename(argc, argv);
	output = outputFilename(argc, argv, input);
	in.open(input);
	out.open(output);
	convert(in, out);
	// Close the files if they are open.
	if (in.is_open())
		in.close();
	if (out.is_open())
		out.close();
	system("pause");
	return 0;
}
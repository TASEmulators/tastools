#include <stdio.h>
#include <iostream>
#include<fstream>

using namespace std;

int main(int argc, char* argv[])
{
	//Get filename from args
	string fn;
	if (argc > 1)
		fn = argv[1];
	else
	{
		cout << "Please specify a filename!\n";
		cout << "\nPress Enter key to exit";	
		cin.ignore(cin.rdbuf()->in_avail()+1);
		return 0;
	}
	///////////////////////////

	//TODO: error check based on extension and file i/o stuff

	ifstream inFile;				//incoming .fm2 file
	inFile.open(fn.c_str());		
	
	string fnout = "";
	fnout.append(fn);
	fnout = fnout.substr(0, (fnout.length()-4));
	fnout.append("-2-controller.fm2");
	
	ofstream outFile;				//outgoing .fm2 file
	outFile.open(fnout.c_str());

	char line[2048];
	string lineStr;

	int count = 1;
	inFile.getline(line,2048);
	while (inFile)
	{
	
		
		if (!(strcmp(line, "fourscore 1")))
			strcpy_s(line, "fourscore 0");
		else if (!(strcmp(line, "port0 0")))
			strcpy_s(line, "port0 1");
		else if (!(strcmp(line, "port1 0")))
			strcpy_s(line, "port1 1");
		else if (!(strcmp(line, "port2 1")))
			strcpy_s(line, "port2 0");
		
		lineStr = line;
		if (line[0] == '|')
		{
			//Funny but it works
			lineStr = lineStr.substr(0, lineStr.find_last_of("|"));
			lineStr = lineStr.substr(0, lineStr.find_last_of("|"));
			lineStr = lineStr.substr(0, lineStr.find_last_of("|"));
			lineStr = lineStr.substr(0, lineStr.find_last_of("|")+1);
		}
		
		outFile << lineStr.c_str() << endl;
		
		count ++;
		if (!(count % 100))
		{
			cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b";
			cout << "Frame: " << count;
		}
		inFile.getline(line,2048);
	}

	return 0;
}
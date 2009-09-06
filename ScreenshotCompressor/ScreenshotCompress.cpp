// SSCompress.cpp
// written by: adelikat
// purpose: to use pngout for optimal compression of game screenshots
// It is designed to be a windows application where the user simply needs to drag & drop the screenshot onto Screenshotcompress.exe

#include <stdio.h>
#include <iostream>
#include <windows.h>
#include <shellapi.h>

using namespace std;

#define sprintf sprintf_s

int main(int argc, char* argv[])
{
	string filename = "test";		//Used as the base name for generating the testArray names
	string filenameFull = filename;
	filenameFull.append(".png");
	bool failure = false;			//used for error checking with filestream, if this becomes true, abort program

//	if (argc > 1)
//	filenameFull = argv[1];		//Get a filename if user chose one
	
	cout << "filename is " << filenameFull.c_str() << "\n"; //debug
	
	string testArray[16];			//Array of filename to test, will be populated based on filename, [0] will be a dummy
	FILE* files[16];				//[0] will be dummy

	////////////////////////////////////////////////////////////////
	// Compress screenshot into 16 files
	////////////////////////////////////////////////////////////////

	//Populate the test array
	char temp[3] = "";
	for (int x = 1; x <= 15; x++)
	{
		testArray[x] = filename;	//Add filename
		
		if (x < 10)
			testArray[x].append("0");	//Make 01, 02 etc.

		sprintf(temp, "%d", x);			//Append X
		testArray[x].append(temp);

		testArray[x].append(".png");
		cout << testArray[x].c_str() << "\n"; //Debug
	}

	//ShellExecuteEx();
	
	////////////////////////////////////////////////////////////////
	//Choose the smallest file
	////////////////////////////////////////////////////////////////
	
	//Open each file and do error checking
	for (int x = 1; x <= 15; x++)
	{
		files[x] = fopen(testArray[x].c_str(), "r");
		if (!files[x])
		{
			failure = true;
			cout << "\n" << testArray[x].c_str() << " file was not found line56";
			break;
		}
	}
	
	if (!failure)
	{
		//Find the lowest filesize
		long lowestSize = 0;
		int lowestNum = 1;
		long tempSize;
		for (int x = 1; x <= 15; x++)
		{
			fseek(files[x], 0, SEEK_END);
			tempSize = ftell(files[x]);
			//cout << "File number " << x << " - " << tempSize << " bytes\n"; //Debug
			if (x == 1)
				lowestSize = tempSize;
			else if (tempSize < lowestSize)
			{
				lowestNum = x;
				lowestSize = tempSize;
			}			
		}

		for (int x = 1; x <= 15; x++)
		{
			fclose(files[x]);
		}
		

		//cout << "\nThe lowest filsize is file number " << lowestNum << "\n" ;	//Debug

		//Delete all but the smallest file
		bool fail = false;
		
		fail = remove(filenameFull.c_str());

		if (fail)
			cout << "Could not remove base file\n";
		
		for (int x = 1; x <= 15; x++)
		{
			if (x != lowestNum)
			{
				fail = remove(testArray[x].c_str());
				if (!fail)
					cout << "Removing file " << x << "\n";
				else
					cout << "Could not remove file " << x << "\n";
			}
		}


		//Rename the lowest to filename
		rename(testArray[lowestNum].c_str(), filenameFull.c_str());
	}
	if (failure)
	{
		cout << "\nCould not open 1 or more files, aborting...";
		cout << "\nPress Enter key to exit";
		cin.ignore(cin.rdbuf()->in_avail()+1);
		return 1;
	}
	//Keep window open until user presses enter
	cout << "\nPress Enter key to exit";
	cin.ignore(cin.rdbuf()->in_avail()+1);

	return 0;
}
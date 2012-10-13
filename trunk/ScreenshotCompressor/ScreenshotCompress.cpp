// SSCompress.cpp
// written by: adelikat & sgrunt
// purpose: to use pngout/deflopt for optimal compression of game screenshots
// It is designed to be a windows application where the user simply needs to drag & drop the screenshot onto Screenshotcompress.exe

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <windows.h>
#include <Winbase.h>
#include <shellapi.h>
   
using namespace std;

#define sprintf sprintf_s

#define DEFAULT_COUNT 50

int main(int argc, char* argv[])
{
	bool failure = false;			//used for error checking with filestream, if this becomes true, abort program
	string filename = "test";		//The dafult if nothing is specified by the user
	string filenameFull;			//Will store the full path to the file that was compressed

	unsigned int count = DEFAULT_COUNT; // number of trials
	
	string **testArray;			//Array of filename to test, will be populated based on filename, [0] will be a dummy
	FILE* thisFile = NULL;

	//Get base directory
	char* getPath = (char*)malloc(2048*sizeof(char));
	GetModuleFileNameA(NULL, getPath, 2048);

	string baseDirectory = getPath;
	char slash = '\\';
	int x = baseDirectory.find_last_of(slash);
	baseDirectory = baseDirectory.substr(0,x+1);

	string cfgpath = baseDirectory;
	cfgpath = cfgpath.append("ScreenshotCompress.cfg");

	string pngoutPath = baseDirectory;
	pngoutPath = pngoutPath.append("pngout.exe");

	string defloptPath = baseDirectory;
	defloptPath = defloptPath.append("deflopt.exe");

	FILE*  configFile = fopen(cfgpath.c_str(), "r");
	if (configFile)
	{
		fscanf(configFile, "%d", &count);
		fclose(configFile);
		if (count > 65535)
		{
			cout << "Please use a sane count value (lower than 65535)!\n";
			return 2;
		}
		cout << "Using " << count << " trials.\n";
	}
	else
	{
		cout << "ScreenshotCompress.cfg not found - defaulting to "
			 << DEFAULT_COUNT << " trials.\n";
	}

	configFile = fopen(pngoutPath.c_str(), "r");
	if (configFile)
	{
		fclose(configFile);
	}
	else
	{
		cout << "pngout.exe not found!\n";
		return 3;
	}

	configFile = fopen(defloptPath.c_str(), "r");
	if (configFile)
	{
		fclose(configFile);
	}
	else
	{
		cout << "deflopt.exe not found!\n";
		return 3;
	}

	if (argc <= 1)
	{
		filenameFull = baseDirectory;
		filenameFull.append(filename);	//If filename isn't specified use test.png in the same dictory as the executable
		filenameFull.append(".png");
	}

	//////////////////////////////////////////////////////////////////////
	for (int z = 1; z < argc; z++)
	{
		testArray = (string **)malloc((count+1)*sizeof(string *));
		for (unsigned int i = 0; i <= count; i++)
			testArray[i] = new string;

		cout << "Working directory is:\n" << baseDirectory.c_str() << "\n\n"; //Debug

		filenameFull = argv[z];

		cout << "Filename is:\n" << filenameFull.c_str() << "\n\n"; //debug
		////////////////////////////////////////////////////////////////
		// Compress screenshot into 16 files
		////////////////////////////////////////////////////////////////

		//Populate the test array
		char temp[6] = "";
		for (unsigned int x = 1; x <= count; x++)
		{
			*testArray[x] = baseDirectory;
			testArray[x]->append(filename);		//Feed pngout the filename
			sprintf(temp, "%05d", x);			//Append X
			testArray[x]->append(temp);
			testArray[x]->append(".png");

			//cout << testArray[x].c_str() << "\n"; //Debug

			string parameters = "\"";
			parameters.append(filenameFull.c_str());	//Send pngout the filename
			parameters.append("\" \"");					//Enclose in quotes
			parameters.append(*testArray[x]);
			parameters.append("\" -r -y");

			cout << "Running pngout " << parameters.c_str() << "\n";

			SHELLEXECUTEINFOA ShExecInfo = {0};
			ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecInfo.hwnd = NULL;
			ShExecInfo.lpVerb = NULL;
			ShExecInfo.lpFile = "pngout.exe";		
			ShExecInfo.lpParameters = parameters.c_str();
			ShExecInfo.lpDirectory = baseDirectory.c_str();
			ShExecInfo.nShow = SW_HIDE;
			ShExecInfo.hInstApp = NULL;	
			ShellExecuteExA(&ShExecInfo);
			WaitForSingleObject(ShExecInfo.hProcess,INFINITE);

			parameters = "\"";
			parameters.append(*testArray[x]); //Send deflopt the filename
			parameters.append("\"");

			cout << "Running deflopt " << parameters.c_str() << "\n";

			ShExecInfo.lpFile = "deflopt.exe";		
			ShExecInfo.lpParameters = parameters.c_str();
			ShellExecuteExA(&ShExecInfo);
			WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
		}

	
	
		////////////////////////////////////////////////////////////////
		//Choose the smallest file
		////////////////////////////////////////////////////////////////

		unsigned long lowestSize = 0, tempSize = 0;
		unsigned short lowestNum = 1;
	
		//Open each file, error check, and find smallest file.
		for (unsigned int x = 1; x <= count; x++)
		{
			thisFile = fopen(testArray[x]->c_str(), "r");
			if (!thisFile)
			{
				failure = true;
				cout << "\n" << testArray[x]->c_str() << " file was not found line56";
				break;
			}

			fseek(thisFile, 0, SEEK_END);
			tempSize = ftell(thisFile);

			if (x == 1) lowestSize = tempSize;
			else if (tempSize < lowestSize)
			{
				lowestNum = x;
				lowestSize = tempSize;
			}

			fclose(thisFile);
		}
	
		// If success, rename best file
		if (!failure)
		{
			//cout << "\nThe lowest filsize is file number " << lowestNum << "\n" ;	//Debug

			//Delete all but the smallest file
			int fail = 0;
		
			fail = remove(filenameFull.c_str());

			if (fail)
				cout << "Could not remove base file\n";
		
			for (unsigned int x = 1; x <= count; x++)
			{
				if (x != lowestNum)
				{
					fail = remove(testArray[x]->c_str());
					//if (!fail)
					//	cout << "Removing file " << x << "\n";
					//else
					//	cout << "Could not remove file " << x << "\n";
				}
			}

			//Rename the lowest to filename
			rename(testArray[lowestNum]->c_str(), filenameFull.c_str());
		}

		for (unsigned int i = 0; i <= count; i++)
			delete testArray[i];
		free(testArray);
	
		if (failure)
		{
			cout << "\nCould not open 1 or more files, aborting...";
			cout << "\nPress Enter key to exit";
			cin.ignore(cin.rdbuf()->in_avail()+1);
			return 1;
		}
	}
	//////////////////////////////////////////////////////////////////////

	//Keep window open until user presses enter
	//cout << "\nPress Enter key to exit";
	//cin.ignore(cin.rdbuf()->in_avail()+1);

	return 0;
}
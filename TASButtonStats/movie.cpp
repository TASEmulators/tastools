#include "stdAfx.h"
#include "movie.h"
#include <fstream>

//ctor
CMovie::CMovie(const char* szFile)
{
	nType = 0;
	fPressesPerFrame = 0.0f;
	uiFrames = 0;
	uiPresses = 0;
	uiReleases = 0;

	std::string filetype = std::string(szFile);
	filetype = filetype.substr(filetype.length()-3, 3);
	for (int i = 0; i < 3; ++i)
		if (filetype[i] >= 'a')
			filetype[i] += 'A' - 'a';
	

	if (!filetype.compare(std::string("SMV")))
		nType = SMV;
	else if (!filetype.compare(std::string("FCM")))
		nType = FCM;
	else if (!filetype.compare(std::string("FM2")))
		nType = FM2;
	else if (!filetype.compare(std::string("GMV")))
		nType = GMV;
	else if (!filetype.compare(std::string("VBM")))
		nType = VBM;
	else if (!filetype.compare(std::string("M64")))
		nType = M64;


	for (int i = 0; i < SNESPADS; ++i)
		for (int j = 0; j < SNESBUTTONS; ++j)
			SNESControllers[i].puiButtonPresses[j] = SNESControllers[i].puiFramesHeld[j] = 0;
	for (int i = 0; i < NESPADS; ++i)
		for (int j = 0; j < NESBUTTONS; ++j)
			NESControllers[i].puiButtonPresses[j] = NESControllers[i].puiFramesHeld[j] = 0;
	for (int i = 0; i < GENESISPADS; ++i)
		for (int j = 0; j < GENESISBUTTONS; ++j)
			GENControllers[i].puiButtonPresses[j] = GENControllers[i].puiFramesHeld[j] = 0;
	for (int i = 0; i < GBAPADS; ++i)
		for (int j = 0; j < GBABUTTONS; ++j)
			GBAControllers[i].puiButtonPresses[j] = GBAControllers[i].puiFramesHeld[j] = 0;
	for (int i = 0; i < N64PADS; ++i)
		for (int j = 0; j < N64BUTTONS; ++j)
			N64Controllers[i].puiButtonPresses[j] = N64Controllers[i].puiFramesHeld[j] = 0;

	uiButtonsOnLastFrame = 0;

	for (int i = 0; i < 6; ++i)
		puiFramesWithButtonsHeld[i] = 0;


	switch(nType)
	{
	case SMV:
		ProcessMovieSMV(szFile);
		break;
	case FCM:
		ProcessMovieFCM(szFile);
		break;
	case FM2:
		ProcessMovieFM2(szFile);
		break;
	case GMV:
		ProcessMovieGMV(szFile);
		break;
	case VBM:
		ProcessMovieVBM(szFile);
		break;
	case M64:
		ProcessMovieM64(szFile);
	default:
		// give error message maybe
		break;
	}

	uiFrames == 0 ? fPressesPerFrame = 0 : fPressesPerFrame = uiPresses / (float)uiFrames;
	
}

//toe
CMovie::~CMovie()
{
}
CMovie::CMovie(const CMovie& mov)
{
	/*course = new Hole[gc.GetNumHoles()];
	for (int i = 0; i < gc.GetNumHoles(); i++)
	{
		course[i].par = gc.GetPar(i);
		course[i].score = gc.GetScore(i);
	}
	this->holes = gc.GetNumHoles();*/
}
CMovie& CMovie::operator=(const CMovie& mov)
{
	/*if (this != &gc)
	{
		delete [] this->course;
		course = new Hole[gc.GetNumHoles()];
		for (int i = 0; i < gc.GetNumHoles(); i++)
		{
			course[i].par = gc.GetPar(i);
			course[i].score = gc.GetScore(i);
		}
		this->holes = gc.GetNumHoles();
	}*/
	return *this;
}

//accessors
int CMovie::GetPresses(int button) const
{
	switch(nType)
	{
	case SMV:
		return SNESControllers[0].puiButtonPresses[button];
	case FCM:
	case FM2:
		return NESControllers[0].puiButtonPresses[button];
	case GMV:
		return GENControllers[0].puiButtonPresses[button];
	case VBM:
		return GBAControllers[0].puiButtonPresses[button];
	case M64:
		return N64Controllers[0].puiButtonPresses[button];
	default:
		break;
	}
	return 0;
}
float CMovie::GetPressesPercent(int button) const
{
	switch(nType)
	{
	case SMV:
		return (100.0f * SNESControllers[0].puiButtonPresses[button]) / uiPresses;
	case FCM:
	case FM2:
		return (100.0f * NESControllers[0].puiButtonPresses[button]) / uiPresses;
	case GMV:
		return (100.0f * GENControllers[0].puiButtonPresses[button]) / uiPresses;
	case VBM:
		return (100.0f * GBAControllers[0].puiButtonPresses[button]) / uiPresses;
	case M64:
		return (100.0f * N64Controllers[0].puiButtonPresses[button]) / uiPresses;
	default:
		break;
	}
	return 0.0f;
}
int CMovie::GetHeld(int button) const
{
	switch(nType)
	{
	case SMV:
		return SNESControllers[0].puiFramesHeld[button];
	case FCM:
	case FM2:
		return NESControllers[0].puiFramesHeld[button];
	case GMV:
		return GENControllers[0].puiFramesHeld[button];
	case VBM:
		return GBAControllers[0].puiFramesHeld[button];
	case M64:
		return N64Controllers[0].puiFramesHeld[button];
	default:
		break;
	}
	return 0;
}
float CMovie::GetHeldPercent(int button) const
{
	switch(nType)
	{
	case SMV:
		return (100.0f * SNESControllers[0].puiFramesHeld[button]) / uiFrames;
	case FCM:
	case FM2:
		return (100.0f * NESControllers[0].puiFramesHeld[button]) / uiFrames;
	case GMV:
		return (100.0f * GENControllers[0].puiFramesHeld[button]) / uiFrames;
	case VBM:
		return (100.0f * GBAControllers[0].puiFramesHeld[button]) / uiFrames;
	case M64:
		return (100.0f * N64Controllers[0].puiFramesHeld[button]) / uiFrames;
	default:
		break;
	}
	return 0.0f;
}
float CMovie::HoldsPerPress(int button) const
{
	switch(nType)
	{
	case SMV:
		return SNESControllers[0].puiButtonPresses[button] ? SNESControllers[0].puiFramesHeld[button] / (float)SNESControllers[0].puiButtonPresses[button] : 0.0f;
	case FCM:
	case FM2:
		return NESControllers[0].puiButtonPresses[button] ? NESControllers[0].puiFramesHeld[button] / (float)NESControllers[0].puiButtonPresses[button] : 0.0f;
	case GMV:
		return GENControllers[0].puiButtonPresses[button] ? GENControllers[0].puiFramesHeld[button] / (float)GENControllers[0].puiButtonPresses[button] : 0.0f;
	case VBM:
		return GBAControllers[0].puiButtonPresses[button] ? GBAControllers[0].puiFramesHeld[button] / (float)GBAControllers[0].puiButtonPresses[button] : 0.0f;
	case M64:
		return N64Controllers[0].puiButtonPresses[button] ? N64Controllers[0].puiFramesHeld[button] / (float)N64Controllers[0].puiButtonPresses[button] : 0.0f;
	default:
		break;
	}
	return 0.0f;
}

//modifiers
void CMovie::UpdateButtonDataFCM(byte * bActiveButtons, unsigned int uiDelta)
{
	int nHeldButtons = 0;
	for (int i = 0; i < NESPADS; ++i)
	{
		for (int j = 0; j < NESBUTTONS; ++j)
		{
			if (bActiveButtons[i] & (1 << j))
			{
				++nHeldButtons;
				NESControllers[i].puiFramesHeld[j] += uiDelta;
			}
		}
		if (i == 0)
		{
			if (nHeldButtons > 5)
				nHeldButtons = 5;

			puiFramesWithButtonsHeld[nHeldButtons] += uiDelta;
		}
	}
}
void CMovie::UpdateButtonDataSMV(unsigned short * bActiveButtons)
{
	int nHeldButtons = 0;
	for (int i = 0; i < SNESPADS; ++i)
	{
		for (int j = 0; j < SNESBUTTONS; ++j)
		{
			if (bActiveButtons[i] & (1 << j))
			{
				++nHeldButtons;
				++SNESControllers[i].puiFramesHeld[j];
			}
		}
		if (i == 0)
		{
			if (nHeldButtons > 5)
				nHeldButtons = 5;

			++puiFramesWithButtonsHeld[nHeldButtons];
		}
	}
}
void CMovie::UpdateButtonDataFM2(byte * bActiveButtons)
{
	int nHeldButtons = 0;
	for (int i = 0; i < NESPADS; ++i)
	{
		for (int j = 0; j < NESBUTTONS; ++j)
		{
			if (bActiveButtons[i] & (1 << j))
			{
				++nHeldButtons;
				++NESControllers[i].puiFramesHeld[j];
			}
		}
		if (i == 0)
		{
			if (nHeldButtons > 5)
				nHeldButtons = 5;

			++puiFramesWithButtonsHeld[nHeldButtons];
		}
	}
}
void CMovie::UpdateButtonDataGMV(byte * bActiveButtons)
{
	int nHeldButtons = 0;
	for (int i = 0; i < GENESISPADS; ++i)
	{
		for (int j = 0; j < GENESISBUTTONS; ++j)
		{
			if (bActiveButtons[i] & (1 << j))
			{
				++nHeldButtons;
				++GENControllers[i].puiFramesHeld[j];
			}
		}
		if (i == 0)
		{
			if (nHeldButtons > 5)
				nHeldButtons = 5;

			++puiFramesWithButtonsHeld[nHeldButtons];
		}
	}
}
void CMovie::UpdateButtonDataVBM(unsigned short * bActiveButtons)
{
	int nHeldButtons = 0;
	for (int i = 0; i < GBAPADS; ++i)
	{
		for (int j = 0; j < GBABUTTONS; ++j)
		{
			if (bActiveButtons[i] & (1 << j))
			{
				++nHeldButtons;
				++GBAControllers[i].puiFramesHeld[j];
			}
		}
		if (i == 0)
		{
			if (nHeldButtons > 5)
				nHeldButtons = 5;

			++puiFramesWithButtonsHeld[nHeldButtons];
		}
	}
}
void CMovie::UpdateButtonDataM64(unsigned int * nActiveButtons)
{
	int nHeldButtons = 0;
	for (int i = 0; i < N64PADS; ++i)
	{
		// digital
		for (int j = 0; j < N64BUTTONS; ++j)
		{
			if (j >= 16 && nActiveButtons[i] & (1 << j))
			{
				++nHeldButtons;
				++N64Controllers[i].puiFramesHeld[j];
			}
		}
		//analog
		if ((nActiveButtons[i] & 0x0000FFFF) != 0)
		{
			++nHeldButtons;
			++N64Controllers[i].puiFramesHeld[0];
		}

		if (i == 0)
		{
			if (nHeldButtons > 5) nHeldButtons = 5;
				++puiFramesWithButtonsHeld[nHeldButtons];
		}
	}
}

void CMovie::BinaryEndianFix(unsigned short &flags)
{
	byte first = flags & 0x00FF;
	byte second = flags >> 8;
	flags = (first << 8) | second;
}
void CMovie::BinaryEndianFix(unsigned int &flags)
{
	byte first	= (flags >> 0)	& 0x000000FF;
	byte second = (flags >> 8)	& 0x000000FF;
	byte third	= (flags >> 16)	& 0x000000FF;
	byte fourth	= (flags >> 24)	& 0x000000FF;
	flags = (first << 24) | (second << 16) | (third << 8) | fourth;
}

//others
void CMovie::ProcessMovieFCM(const char* szFile)
{
	byte bCurrButtons[NESPADS] = { 0 };
	//get as much data from the file as we can
	std::ifstream ifl;
	ifl.exceptions(std::ios_base::failbit | std::ios_base::eofbit | std::ios_base::badbit);
	try
	{
		ifl.open(szFile, std::ios_base::in | std::ios_base::binary);
		char cMovieType[4] = { 0 };
		ifl.read(cMovieType, 3);
		if (!strcmp(cMovieType, "FCM"))
		{

			// where does input data start?
			unsigned int uiInputOffset;
			ifl.seekg(0x1C);
			ifl.read((char*)&uiInputOffset, 4);
			ifl.seekg(uiInputOffset);

			// get the data for each frame
			uiFrames = 0;
			while (true)
			{
				// get the update data
				byte bUpdateByte;
				ifl.read((char*)&bUpdateByte, 1);

				// advance delta bytes
				unsigned int uiNumDeltaBytes;
				unsigned int uiDelta = 0;
				uiNumDeltaBytes = (bUpdateByte & 0x60) >> 5;
				ifl.read((char*)(&uiDelta), uiNumDeltaBytes);

				// go to next frame if not a controller update
				if (~bUpdateByte & 0x80)
				{
					// Which controller?
					unsigned int uiController;
					uiController = (bUpdateByte & 0x18) >> 3;

					// Which button to toggle?
					unsigned int uiButton;
					uiButton = (bUpdateByte & 0x07);

					//toggle bit
					bCurrButtons[uiController] ^= (1 << uiButton);
					if ((bCurrButtons[uiController] >> uiButton) & 1)
					{
						++NESControllers[uiController].puiButtonPresses[uiButton];
						++uiPresses;
					}
				}
				else if ((bUpdateByte & 0x1F) != 0)
				{
					// I dunno if this is necessary
					//for (int i = 0; i < NESPADS; ++i)
					//	bCurrButtons[i] = 0;
				}

				// done 
				if (uiDelta)
				{
					uiFrames += uiDelta;
					UpdateButtonDataFCM(bCurrButtons, uiDelta);
				}
			}
		}
		else
		{
			nType = 0;
			// error that the user LIED!
		}
	}
	catch (std::ios_base::failure)
	{
		ifl.close();

		uiReleases = uiPresses;
		uiButtonsOnLastFrame = (unsigned int)bCurrButtons[0];
		for (int i = 0; i < NESPADS; ++i)
			for (int j = 0; j < NESBUTTONS; ++j)
				if (bCurrButtons[i] & (1 << j))
					--uiReleases;
	}
}

void CMovie::ProcessMovieSMV(const char* szFile)
{
	unsigned short usCurrButtons[SNESPADS] = { 0 };
	//get as much data from the file as we can
	std::ifstream ifl;
	ifl.exceptions(std::ios_base::failbit | std::ios_base::eofbit | std::ios_base::badbit);
	try
	{
		ifl.open(szFile, std::ios_base::in | std::ios_base::binary);
		char cMovieType[4] = { 0 };
		ifl.read(cMovieType, 3);
		if (!strcmp(cMovieType, "SMV"))
		{
			// how many frames?
			ifl.seekg(0x10);
			ifl.read((char*)&uiFrames, 4);
			++uiFrames;

			//how many SNESControllers?
			byte bControllerFlag;
			ifl.seekg(0x14);
			ifl.read((char*)&bControllerFlag, 1);
			uiControllers = (bControllerFlag & 1) + ((bControllerFlag >> 1) & 1) + ((bControllerFlag >> 2) & 1) + ((bControllerFlag >> 3) & 1) + ((bControllerFlag >> 4) & 1);

			// where does input data start?
			unsigned int uiInputOffset;
			ifl.seekg(0x1C);
			ifl.read((char*)&uiInputOffset, 4);
			ifl.seekg(uiInputOffset);

			// get the data for each frame
			while (true)
			{
				// for each controller
				for (unsigned int i = 0; i < uiControllers; ++i) 
				{
					// get the frame data
					unsigned short usFrame = 0;
					ifl.read((char*)&usFrame, 2);
					BinaryEndianFix(usFrame);

					// Don't count input if it's a reset
					if (usFrame != 0xFFFF)
					{
						// Which button to toggle?
						unsigned short usButtonToggle = usFrame ^ usCurrButtons[i];
						usCurrButtons[i] = usFrame;
						
						for (int j = 0; j < SNESBUTTONS; ++j)
						{
							if ((usButtonToggle & (1 << j)) && usFrame & (1 << j))
							{
								++SNESControllers[i].puiButtonPresses[j];
								if (i == 0)
									++uiPresses;
							}
						}
					}
					else
					{
						usCurrButtons[i] = 0;
					}
				}
				// done
				UpdateButtonDataSMV(usCurrButtons);
			}
		}
		else
		{
			nType = 0;
			// error message that the user LIED!
		}
	}
	catch (std::ios_base::failure)
	{
		ifl.close();

		uiReleases = uiPresses;
		uiButtonsOnLastFrame = (unsigned int)usCurrButtons[0];
		for (int i = 0; i < SNESPADS; ++i)
			for (int j = 0; j < SNESBUTTONS; ++j)
				if (usCurrButtons[i] & (1 << j))
					--uiReleases;
	}
}

void CMovie::ProcessMovieFM2(const char* szFile)
{
	byte bCurrButtons[NESPADS] = { 0 };
	//get as much data from the file as we can
	std::ifstream ifl;
	ifl.exceptions(std::ios_base::failbit | std::ios_base::eofbit | std::ios_base::badbit);
	uiFrames = 0;
	try
	{
		ifl.open(szFile, std::ios_base::in);

		// get the data for each frame
		while (true)
		{
			int frameflag;
			char c;
			char szController1[NESBUTTONS+1] = { 0 };

			while((c=ifl.peek())!='|')ifl.get();	//	preceeding frame remainder
			ifl.get();								//	|
			ifl >> frameflag;						//	1 for reset, probably 0
			ifl.get();								//	|
			ifl.read(szController1, NESBUTTONS);	//	port0 data
			ifl.get();								//	|
			while((c=ifl.peek())!='|') ifl.get();	//	port1 data
			ifl.get();								//	|
			while((c=ifl.peek())!='|') ifl.get();	//	port2 data
			ifl.get();								//	|

			// convert the frame data
			byte bController1 = 0;
			for (int i = 0; i < NESBUTTONS; ++i)
			{
				if (szController1[i] != ' ' && szController1[i] != '.')
					bController1 |= (0x80 >> i);
			}

			// Which button to toggle?
			byte bButtonToggle = bController1 ^ bCurrButtons[0];
			bCurrButtons[0] = bController1;

			for (int j = 0; j < NESBUTTONS; ++j)
			{
				if ((bButtonToggle & (1 << j)) && bController1 & (1 << j))
				{
					++NESControllers[0].puiButtonPresses[j];
					++uiPresses;
				}
			}


			//// done
			UpdateButtonDataFM2(bCurrButtons);
			++uiFrames;
		}
	}
	catch (std::ios_base::failure)
	{
		ifl.close();

		uiReleases = uiPresses;
		uiButtonsOnLastFrame = (unsigned int)bCurrButtons[0];
		for (int i = 0; i < NESPADS; ++i)
			for (int j = 0; j < NESBUTTONS; ++j)
				if (bCurrButtons[i] & (1 << j))
					--uiReleases;
	}

	if (ifl.is_open())
		ifl.close();
}

void CMovie::ProcessMovieGMV(const char* szFile)
{
	byte bCurrButtons[GENESISPADS] = { 0 };
	//get as much data from the file as we can
	std::ifstream ifl;
	ifl.exceptions(std::ios_base::failbit | std::ios_base::eofbit | std::ios_base::badbit);
	try
	{
		ifl.open(szFile, std::ios_base::in | std::ios_base::binary);
		char cMovieType[9] = { 0 };
		ifl.read(cMovieType, 8);
		if (!strcmp(cMovieType, "Gens Mov"))
		{
			// where does input data start?
			ifl.seekg(0x40);

			// get the data for each frame
			uiFrames = 0;
			while (true)
			{
				// for each controller
				for (unsigned int i = 0; i < GENESISPADS; ++i) 
				{
					// get the frame data
					byte bFrame;
					ifl.read((char*)&(bFrame), 1);
					bFrame = ~bFrame;

					// Don't count input if it's a reset
					if (bFrame != 0xFF)
					{
						// Which button to toggle?
						byte bButtonToggle = bFrame ^ bCurrButtons[i];
						bCurrButtons[i] = bFrame;
						
						for (int j = 0; j < GENESISBUTTONS; ++j)
						{
							if ((bButtonToggle & (1 << j)) && bFrame & (1 << j))
							{
								++GENControllers[i].puiButtonPresses[j];
								if (i == 0)
									++uiPresses;
							}
						}
					}
					else
					{
						bCurrButtons[i] = 0;
					}
				}
				// done
				UpdateButtonDataGMV(bCurrButtons);
				++uiFrames;
			}
		}
		else
		{
			nType = 0;
			// error message that the user LIED!
		}
	}
	catch (std::ios_base::failure)
	{
		ifl.close();

		uiReleases = uiPresses;
		uiButtonsOnLastFrame = (unsigned int)bCurrButtons[0];
		for (int i = 0; i < GENESISPADS; ++i)
			for (int j = 0; j < GENESISBUTTONS; ++j)
				if (bCurrButtons[i] & (1 << j))
					--uiReleases;
	}

	if (ifl.is_open())
		ifl.close();
}

void CMovie::ProcessMovieVBM(const char* szFile)
{
	unsigned short usCurrButtons[GBAPADS] = { 0 };
	//get as much data from the file as we can
	std::ifstream ifl;
	ifl.exceptions(std::ios_base::failbit | std::ios_base::eofbit | std::ios_base::badbit);
	try
	{
		ifl.open(szFile, std::ios_base::in | std::ios_base::binary);
		char cMovieType[4] = { 0 };
		ifl.read(cMovieType, 3);
		if (!strcmp(cMovieType, "VBM"))
		{
			// how many frames?
			ifl.seekg(0x0C);
			ifl.read((char*)&uiFrames, 4);

			//how many GBAControllers?
			byte bControllerFlag;
			ifl.seekg(0x15);
			ifl.read((char*)&bControllerFlag, 1);
			uiControllers = (bControllerFlag & 1) + ((bControllerFlag >> 1) & 1) + ((bControllerFlag >> 2) & 1) + ((bControllerFlag >> 3) & 1);

			// where does input data start?
			unsigned int uiInputOffset;
			ifl.seekg(0x3C);
			ifl.read((char*)&uiInputOffset, 4);
			ifl.seekg(uiInputOffset);

			// get the data for each frame
			while (true)
			{
				// for each controller
				for (unsigned int i = 0; i < uiControllers; ++i) 
				{
					// get the frame data
					unsigned short usFrame = 0;
					ifl.read((char*)&usFrame, 2);
					//BinaryEndianFix(usFrame); <-- what! the endianness is correct????

					// Don't count input if it's a reset
					if (~usFrame & 0x0400)
					{
						// Which button to toggle?
						unsigned short usButtonToggle = usFrame ^ usCurrButtons[i];
						usCurrButtons[i] = usFrame;
						
						for (int j = 0; j < GBABUTTONS; ++j)
						{
							if ((usButtonToggle & (1 << j)) && usFrame & (1 << j))
							{
								++GBAControllers[i].puiButtonPresses[j];
								if (i == 0)
									++uiPresses;
							}
						}
					}
					else
					{
						usCurrButtons[i] = 0;
					}
				}
				// done
				UpdateButtonDataVBM(usCurrButtons);
			}
		}
		else
		{
			nType = 0;
			// error message that the user LIED!
		}
	}
	catch (std::ios_base::failure)
	{
		ifl.close();

		uiReleases = uiPresses;
		uiButtonsOnLastFrame = (unsigned int)usCurrButtons[0];
		for (int i = 0; i < GBAPADS; ++i)
			for (int j = 0; j < GBABUTTONS; ++j)
				if (usCurrButtons[i] & (1 << j))
					--uiReleases;
	}

	if (ifl.is_open())
		ifl.close();
}

void CMovie::ProcessMovieM64(const char* szFile)
{
	unsigned int nCurrButtons[N64PADS] = { 0 };
	//get as much data from the file as we can
	std::ifstream ifl;
	ifl.exceptions(std::ios_base::failbit | std::ios_base::eofbit | std::ios_base::badbit);
	try
	{
		ifl.open(szFile, std::ios_base::in | std::ios_base::binary);
		char cMovieType[4] = { 0 };
		ifl.read(cMovieType, 3);
		if (!strcmp(cMovieType, "M64"))
		{
			// Version Number
			unsigned int nVersion;
			ifl.seekg(0x4);
			ifl.read((char*)&nVersion, 4);

			//how many N64Controllers?
			byte bControllerFlag;
			ifl.seekg(0x15);
			ifl.read((char*)&bControllerFlag, 1);
			uiControllers = bControllerFlag;

			// where does input data start?
			switch(nVersion)
			{
			case 1:
			case 2:
				ifl.seekg(0x200);
				break;
			case 3:
				ifl.seekg(0x400);
				break;
			default:
				ifl.seekg(0x400);
				break;
			}

			// get the data for each frame
			uiFrames = 0;
			while (true)
			{
				// for each controller
				for (unsigned int i = 0; i < uiControllers; ++i) 
				{
					// get the frame data
					unsigned int nFrame = 0;
					ifl.read((char*)&nFrame, 4);
					BinaryEndianFix(nFrame);

					// Don't count input if it's a reset
					if (nFrame != 0x00C00000) // this might be wrong, it might be ((nFrame & 0x0000C000) == 0)
					{
						// Which button to toggle?
						unsigned int nButtonToggle = nFrame ^ nCurrButtons[i];
						
						// digital buttons
						for (int j = 0; j < N64BUTTONS; ++j)
						{
							if (j >= 16 && (nButtonToggle & (1 << j)) && nFrame & (1 << j))
							{
								++N64Controllers[i].puiButtonPresses[j];
								if (i == 0)
									++uiPresses;
							}
						}
						// analog stick (nframe is this frame, currbuttons is the previous)
						if ((nCurrButtons[i] & 0x0000FFFF) == 0 && (nFrame & 0x0000FFFF) != 0)
						{
							++N64Controllers[i].puiButtonPresses[0];
							if (i == 0)
								++uiPresses;
						}
						nCurrButtons[i] = nFrame;
					}
					else
					{
						nCurrButtons[i] = 0;
					}
				}
				// done
				UpdateButtonDataM64(nCurrButtons);
				++uiFrames;
			}
		}
		else
		{
			nType = 0;
			// error message that the user LIED!
		}
	}
	catch (std::ios_base::failure)
	{
		ifl.close();

		uiReleases = uiPresses;
		uiButtonsOnLastFrame = nCurrButtons[0];
		for (int i = 0; i < N64PADS; ++i)
		{
			if (nCurrButtons[i] & 0x0000FFFF)
				--uiReleases;

			for (int j = 0; j < N64BUTTONS; ++j)
				if (j > 15 && nCurrButtons[i] & (1 << j))
					--uiReleases;
		}
	}

	if (ifl.is_open())
		ifl.close();
}
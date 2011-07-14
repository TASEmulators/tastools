#pragma once
typedef unsigned char byte;
#include <vector>

enum eTYPE { FCM = 1, FM2, SMV, GMV, VBM, M64, MAX_TYPES};
const int SNESBUTTONS = 16;
const int SNESPADS = 5;
const int NESBUTTONS = 8;
const int NESPADS = 4;
const int GENESISBUTTONS = 8;
const int GENESISPADS = 3;
const int N64BUTTONS = 32;
const int N64PADS = 4;
const int GBABUTTONS = 16;
const int GBAPADS = 4;

class CMovie
{
private:
	int nType;
	unsigned int uiFrames;
	unsigned int uiPresses;
	unsigned int uiReleases;
	unsigned int uiButtonsOnLastFrame;
	float fPressesPerFrame;
	unsigned int puiFramesWithButtonsHeld[6];

	unsigned int uiControllers;

	struct _controllersnes
	{
		unsigned int puiButtonPresses[SNESBUTTONS];
		unsigned int puiFramesHeld[SNESBUTTONS];
	} SNESControllers[SNESPADS];
	struct _controllergen
	{
		unsigned int puiButtonPresses[GENESISBUTTONS];
		unsigned int puiFramesHeld[GENESISBUTTONS];
	} GENControllers[GENESISPADS];
	struct _controllernes
	{
		unsigned int puiButtonPresses[NESBUTTONS];
		unsigned int puiFramesHeld[NESBUTTONS];
	} NESControllers[NESPADS];
	struct _controllern64
	{
		unsigned int puiButtonPresses[N64BUTTONS];
		unsigned int puiFramesHeld[N64BUTTONS];
	} N64Controllers[N64PADS];
	struct _controllergba
	{
		unsigned int puiButtonPresses[GBABUTTONS];
		unsigned int puiFramesHeld[GBABUTTONS];
	} GBAControllers[GBAPADS];


public:
	//ctor
	CMovie(const char* szFile);

	//toe
	~CMovie();
	CMovie(const CMovie& mov);
	CMovie& operator=(const CMovie& mov);

	//accessors
	int GetType() const { return nType; }
	float GetPPF() const { return fPressesPerFrame; }
	int GetTotalPresses() const { return uiPresses; }
	int GetTotalReleases() const { return uiReleases; }
	int GetFrames() const { return uiFrames; }
	int GetPresses(int button) const;
	float GetPressesPercent(int button) const;
	int GetHeld(int button) const;
	float GetHeldPercent(int button) const;
	float HoldsPerPress(int button) const;

	int GetFramesWithButtonsHeld(int num) const { return puiFramesWithButtonsHeld[num]; }
	float GetFramesWithButtonsHeldPercent(int num) const { return (100.0f * puiFramesWithButtonsHeld[num]) / uiFrames; }

	//modifiers
	void UpdateButtonDataFCM(byte * bActiveButtons, unsigned int delta);
	void UpdateButtonDataSMV(unsigned short * bActiveButtons);
	void UpdateButtonDataFM2(byte * bActiveButtons);
	void UpdateButtonDataGMV(byte * bActiveButtons);
	void UpdateButtonDataVBM(unsigned short * bActiveButtons);
	void UpdateButtonDataM64(unsigned int * nActiveButtons);

	//others
	void ProcessMovieFCM(const char* szFile);
	void ProcessMovieSMV(const char* szFile);
	void ProcessMovieFM2(const char* szFile);
	void ProcessMovieGMV(const char* szFile);
	void ProcessMovieVBM(const char* szFile);
	void ProcessMovieM64(const char* szFile);

	void BinaryEndianFix(unsigned short &flags);
	void BinaryEndianFix(unsigned int &flags);
};

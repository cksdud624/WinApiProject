#define _CRT_SECURE_NO_WARNINGS
#include "SoundManager.h"
#include <iostream>
#include <Windows.h>
#include <tchar.h>
using namespace std;

void SoundManager::Init(HWND& hWnd)
{
	this->hWnd = hWnd;
}

void SoundManager::LoadWav(LPCTSTR wav)
{
	DWORD Result;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;
	mciOpenParms.lpstrElementName = wav;
	mciOpenParms.lpstrDeviceType = L"WaveAudio";
	UINT wDeviceID = 0;
	Result = mciSendCommand(wDeviceID, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)(LPVOID) & mciOpenParms);

	wDeviceID = mciOpenParms.wDeviceID;

	mciPlayParms.dwCallback = (DWORD)hWnd;


	mciSendCommand(wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID) & mciPlay);


}

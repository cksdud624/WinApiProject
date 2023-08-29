#pragma once
#include "Singleton.h"
#include <vector>
#include <thread>
#include <future>
#include <iostream>
#include <chrono>
#include <string>
#include <windows.h>
#include <mmsystem.h>
#include <tchar.h>
#pragma comment(lib, "winmm.lib")

using namespace std;
class SoundManager;

class SoundManager : public Singleton<SoundManager>
{
private:
	MCI_PLAY_PARMS       mciPlay; //파일을 재생
	MCI_STATUS_PARMS   mciStatus; //파일의 상태
	HWND hWnd;
	HANDLE hThread;

public:
	vector<LPCTSTR> soundtypes;
	void Init(HWND& hWnd);

	void playSound(LPCTSTR command) { soundtypes.push_back(command); }

	void LoadWav(LPCTSTR wav);
};
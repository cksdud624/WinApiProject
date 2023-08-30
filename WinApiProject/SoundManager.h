#pragma once
#include "Singleton.h"
#include <vector>
#include <thread>
#include <future>
#include <iostream>
#include <string>
#include <windows.h>
#include <tchar.h>
#include "CSound.h"

using namespace std;
class SoundManager;

class SoundManager : public Singleton<SoundManager>
{
private:
	vector<CSound*> sounds;
	vector<string> names;
public:
	void addSound(string newsound);
	void addBackground(string background);
	void playSound(string sound);
	void playThread(string sound);
	void lowerVolume(string sound);
	void stopSound(string sound);
};
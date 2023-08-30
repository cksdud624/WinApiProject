#include "SoundManager.h"
#include <iostream>
#include <Windows.h>
#include <tchar.h>
#include <string>
using namespace std;

void SoundManager::addSound(string newsound)
{
	char tempch[100];
	strcpy_s(tempch, newsound.c_str());
	CSound* temp = new CSound(tempch, false);
	sounds.push_back(temp);
	names.push_back(newsound);
}

void SoundManager::playSound(string sound)
{
	thread t([this, sound]() {
		this->playThread(sound);
		});

	t.join();
}

void SoundManager::playThread(string sound)
{
	int index = -1;
	for (int i = 0; i < sounds.size(); i++)
	{
		if (names[i] == sound)
		{
			index = i;
			break;
		}
	}

	sounds[index]->play();
	sounds[index]->Update();
}

void SoundManager::lowerVolume(string sound)
{
	int index = -1;
	for (int i = 0; i < sounds.size(); i++)
	{
		if (names[i] == sound)
		{
			index = i;
			break;
		}
	}

	sounds[index]->volumeDown();
}

void SoundManager::stopSound(string sound)
{
	int index = -1;
	for (int i = 0; i < sounds.size(); i++)
	{
		if (names[i] == sound)
		{
			index = i;
			break;
		}
	}

	sounds[index]->stop();
}

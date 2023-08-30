#pragma once
#include <iostream>
using namespace std;


template <typename T>
class Singleton
{
private:
	static T* instance;
protected:
	Singleton() {}
public:
	static T* getInstance()
	{
		if (!instance)
		{
			instance = new T;
		}
		return instance;
	}
	static void DestroyInstance()
	{
		if (instance)
		{
			delete instance;
			instance = nullptr;
		}
	}
};

template<typename T> T* Singleton<T>::instance = nullptr;
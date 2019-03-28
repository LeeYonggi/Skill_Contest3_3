#pragma once

template<class T>
class Singleton
{
public:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static T *instance;

public:
	static T *GetInstance()
	{
		if (!instance)
			instance = new T;
		return instance;
	}
	static void ReleaseInstance()
	{
		if (instance)
			delete instance;
	}
};

template<class T>
T *Singleton<T>::instance = nullptr;
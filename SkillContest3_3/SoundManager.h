#pragma once
#include "Singleton.h"
#include "SDKsound.h"

class SoundManager :
	public Singleton<SoundManager>
{
public:
	SoundManager();
	virtual ~SoundManager();


private:
	CSoundManager *manager;	
	map<string, CSound*> m_Sound;

public:
	string CreateSound(string str, wstring route);
	void DuplicatePlay(string str);
	void PlayCSound(string str, bool isLoop);
	void StopSound(string str);
};

#define SOUNDMANAGER SoundManager::GetInstance()
#include "DXUT.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	manager = new CSoundManager;
	manager->Initialize(DXUTGetHWND(), 2);
}


SoundManager::~SoundManager()
{
	for (auto iter : m_Sound)
	{
		SAFE_DELETE(iter.second);
	}
	m_Sound.clear();
	SAFE_DELETE(manager);
}

string SoundManager::CreateSound(string str, wstring route)
{
	CSound *sound;
	manager->Create(&sound, (LPWSTR)route.c_str(), 0, GUID_NULL);

	m_Sound.insert(make_pair(str, sound));
	return str;
}

void SoundManager::DuplicatePlay(string str)
{
	LPDIRECTSOUNDBUFFER buffer;

	manager->GetDirectSound()->DuplicateSoundBuffer(m_Sound[str]->GetBuffer(0), &buffer);

	buffer->SetCurrentPosition(0);
	buffer->Play(0, 0, 0);
}

void SoundManager::PlayCSound(string str, bool isLoop)
{
	m_Sound[str]->Play(0, isLoop);
}

void SoundManager::StopSound(string str)
{
	m_Sound[str]->Stop();
}

#include "DXUT.h"
#include "DataManager.h"


DataManager::DataManager()
{
	point = 0;
}


DataManager::~DataManager()
{
}

void DataManager::Init()
{
	animeTexture = IMAGEMANAGER->AddAnimeTexture("SceneChange", "./Resource/scene/scene_%d.png", 1, 17);
	frame = 0;
	isSceneChange = false;
}

void DataManager::Update()
{
	if(isSceneChange)
		frame += 0.015;
}

void DataManager::Render()
{
	if (!isSceneChange) return;
	int count = int(frame * 12) % animeTexture.size();
	if (frame * 12 >= animeTexture.size())
	{
		count = animeTexture.size() - 1;
		isSceneChange = false;
	}
	IMAGEMANAGER->DrawTexture(animeTexture[count], { 640, 360});
}

void DataManager::StartSceneChange()
{
	frame = 0;
	isSceneChange = true;
}

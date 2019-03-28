#pragma once
#include "Singleton.h"
class DataManager :
	public Singleton<DataManager>
{
public:
	DataManager();
	virtual ~DataManager();

public:
	Vector3 savePoint;
	int point;

private:
	vector<Texture*> animeTexture;
	float frame;
	bool isSceneChange;

public:
	void Init();
	void Update();
	void Render();
	void StartSceneChange();
};

#define DATAMANAGER DataManager::GetInstance()
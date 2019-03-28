#pragma once
#include "Scene.h"
#include <thread>

#define ANIMEFRAME 10
class LoadScene :
	public Scene
{
public:
	LoadScene();
	virtual ~LoadScene();

private:
	thread meshThread;
	vector<Texture*> animeTexture;
	bool isEndThread = false;
	float frame;

public:
	void Init();
	void Update();
	void Render();
	void Release();

public:
	void Loading();
};


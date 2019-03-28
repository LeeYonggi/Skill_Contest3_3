#pragma once
#include "Scene.h"

enum STAGE_STATE
{
	STAGE_1,
	STAGE_2 
};
class IngameScene :
	public Scene
{
public:
	IngameScene(STAGE_STATE _stage);
	virtual ~IngameScene();

private:
	STAGE_STATE stage;
	string soundKey;
public:
	void Init();
	void Update();
	void Render();
	void Release();
};


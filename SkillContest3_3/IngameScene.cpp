#include "DXUT.h"
#include "IngameScene.h"

#include "Player.h"
#include "TsCamera.h"
#include "PlayerArm.h"
#include "Background.h"
#include "Enemy.h"
#include "AirEnemy1.h"
#include "Rock.h"
#include "ObjectCreater.h"
#include "SDKsound.h"


IngameScene::IngameScene(STAGE_STATE _stage)
{
	stage = _stage;
}


IngameScene::~IngameScene()
{
}

void IngameScene::Init()
{
	soundKey = SOUNDMANAGER->CreateSound("InGame", L"./Sound/XACTGameGroove2.wav");
	SOUNDMANAGER->PlayCSound(soundKey, true);
	OBJECTMANAGER->AddObject(OBJ_CREATER, new ObjectCreater(stage));
}

void IngameScene::Update()
{
}

void IngameScene::Render()
{
}

void IngameScene::Release()
{
	SOUNDMANAGER->StopSound(soundKey);
}

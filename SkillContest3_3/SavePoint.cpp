#include "DXUT.h"
#include "SavePoint.h"

#include "Player.h"
#include "IngameScene.h"
#include "EndingScene.h"

SavePoint::SavePoint(Player *_player, Vector3 _pos, STAGE_STATE _stage, float _isLast)
{
	player = _player;
	radius = 5;
	pos = _pos;
	stage = _stage;
	isLast = _isLast;
}


SavePoint::~SavePoint()
{
}

void SavePoint::Init()
{
	mesh = MESHMANAGER->AddMesh("save_point", L"./Resource/save_point/save_point.obj");
	
	scale = { 0.1, 0.1, 0.1 };
}

void SavePoint::Update()
{
	if (GetCircleCollision(pos, player->pos, radius, player->radius))
	{
		if (isLast)
		{
			switch (stage)
			{
			case STAGE_1:
				SCENEMANAGER->AddScene(new IngameScene(STAGE_2));
				break;
			case STAGE_2:
				SCENEMANAGER->AddScene(new EndingScene(true));
				break;
			default:
				break;
			}
		}
		DATAMANAGER->savePoint = pos;
	}
}

void SavePoint::Render()
{
	MESHMANAGER->DrawAlphaMesh(mesh, pos, matR, scale);
}

void SavePoint::Release()
{
}

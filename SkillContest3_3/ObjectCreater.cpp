#include "DXUT.h"
#include "ObjectCreater.h"

#include "Player.h"
#include "TsCamera.h"
#include "PlayerArm.h"
#include "Background.h"
#include "Enemy.h"
#include "AirEnemy1.h"
#include "Rock.h"
#include "SavePoint.h"
#include "Particle.h"
#include "AirEnemy2.h"
#include "MainScene.h"
#include "EndingScene.h"



ObjectCreater::ObjectCreater(STAGE_STATE _stage)
{
	stage = _stage;
}

ObjectCreater::~ObjectCreater()
{
}

void ObjectCreater::Init()
{
	Background *background;
	Player *player;
	int i = 0;
	switch (stage)
	{
	case STAGE_1:
		background = OBJECTMANAGER->AddObject(OBJ_BACKGROUND, new Background(stage));
		player = OBJECTMANAGER->AddObject(OBJ_PLAYER, new Player(background));
		OBJECTMANAGER->AddObject(OBJ_PLAYER, new PlayerArm(player));
		for (int i = 0; i < 25; i++)
		{
			int enemyKind = d3d::GetRandomNumber(0, 9);
			if (enemyKind < 7)
			{
				float enemyNumber = d3d::GetRandomNumber(-30, 30);
				OBJECTMANAGER->AddObject(OBJ_ENEMY, new AirEnemy1(player, { 100 + (float)i * 25, 20, enemyNumber }));
			}
			else
			{
				float enemyNumber = d3d::GetRandomNumber(-30, 30);
				OBJECTMANAGER->AddObject(OBJ_ENEMY, new AirEnemy2(player, { 100 + (float)i * 25, 20, enemyNumber }));
			}
		}
		i = 0;
		while (true)
		{
			float rockNumber = d3d::GetRandomNumber(-30, 30);
			Vector2 temp;
			temp.x = 40 + (float)i * 20;
			temp.y = rockNumber;
			if (GetPixelCollision(temp, background->minimap2))
				OBJECTMANAGER->AddObject(OBJ_ENEMY, new Rock(player, { temp.x, 0, temp.y }));
			else
				continue;
			i++;
			if (i > 24)
				break;
		}
		DATAMANAGER->savePoint = { 10, 0, 0 };
		CAMERAMANAGER->SetCamera(OBJECTMANAGER->AddObject(OBJ_CAMERA, new TsCamera(player)));

		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 20, 0, 0}	 , stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 100, 0, 0 }  , stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 200, 0, 0 }  , stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 240, 0, -20 }, stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 460, 0, 0 }  , stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 540, 0, 0 }, stage, true));
		break;
	case STAGE_2:
		background = OBJECTMANAGER->AddObject(OBJ_BACKGROUND, new Background(stage));
		player = OBJECTMANAGER->AddObject(OBJ_PLAYER, new Player(background));
		OBJECTMANAGER->AddObject(OBJ_PLAYER, new PlayerArm(player));
		for (int i = 0; i < 25; i++)
		{
			float enemyNumber = d3d::GetRandomNumber(-30, 30);
			OBJECTMANAGER->AddObject(OBJ_ENEMY, new AirEnemy1(player, { 100 + (float)i * 25, 20, enemyNumber }));
		}
		i = 0;
		while (true)
		{
			float rockNumber = d3d::GetRandomNumber(-110, 110);
			Vector2 temp;
			temp.x = 40 + (float)i * 20;
			temp.y = rockNumber;
			if (GetPixelCollision(temp, background->minimap2))
				OBJECTMANAGER->AddObject(OBJ_ENEMY, new Rock(player, { temp.x, 0, temp.y }));
			else
				continue;
			i++;
			if (i > 24)
				break;
		}
		DATAMANAGER->savePoint = { 10, 0, 0 };
		CAMERAMANAGER->SetCamera(OBJECTMANAGER->AddObject(OBJ_CAMERA, new TsCamera(player)));

		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 10, 0, 3 }, stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 60, 0, 50 }, stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 200, 0, 50 }, stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 410, 0, 100 }, stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 540, 0, 50 }, stage, false));
		OBJECTMANAGER->AddObject(OBJ_SAVEPOINT, new SavePoint(player, { 630, 0, 14 }, stage, true));
		break;
	default:
		break;
	}
	vParticle.push_back(OBJECTMANAGER->AddObject(OBJ_EFFECT, new Particle("poison", 1, 9, 0.2f, 10.0f)));
	vParticle[0]->isActive = true;
}

void ObjectCreater::Update()
{
	if (INPUTMANAGER->KeyDown(VK_F3))
	{
		SCENEMANAGER->AddScene(new EndingScene(true));
	}
	if (INPUTMANAGER->KeyDown(VK_F4))
	{
		SCENEMANAGER->AddScene(new MainScene());
	}
	if (INPUTMANAGER->KeyDown(VK_F5))
	{
		SCENEMANAGER->AddScene(new IngameScene(STAGE_1));
	}
	if (INPUTMANAGER->KeyDown(VK_F6))
	{
		SCENEMANAGER->AddScene(new IngameScene(STAGE_2));
	}
	Vector3 temp = { 0, -10, 0 };
	temp.x = d3d::GetRandomNumber(0, 600);
	temp.z = d3d::GetRandomNumber(-100, 500);
	moveVector.x = d3d::GetRandomNumber(-5, 5) * 0.1f;
	moveVector.z = d3d::GetRandomNumber(-5, 5) * 0.1f;
	moveVector.y = 1;
	float multi = d3d::GetRandomNumber(10, 30) * 0.1f;
	float tempScale = d3d::GetRandomNumber(40, 70);
	vParticle[0]->ParticleInit(temp, moveVector * multi, tempScale * 0.001f, true);
}

void ObjectCreater::Render()
{
}

void ObjectCreater::Release()
{
}

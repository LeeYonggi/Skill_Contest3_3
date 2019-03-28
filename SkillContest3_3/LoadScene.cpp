#include "DXUT.h"
#include "LoadScene.h"

#include "IngameScene.h"
#include "MainScene.h"

LoadScene::LoadScene()
{
}


LoadScene::~LoadScene()
{
}

void LoadScene::Init()
{
	meshThread = thread([&]() { Loading(); });
	animeTexture = IMAGEMANAGER->AddAnimeTexture("Loading", "./Resource/Loading/%d.png", 1, 3);
}

void LoadScene::Loading()
{
	Sleep(5);
	//Intro
	IMAGEMANAGER->AddAnimeTexture("Intro", "./Resource/intro/intro%04d.jpg", 0, 50);

	//Sound
	SOUNDMANAGER->CreateSound("fire", L"./Sound/ammo_fire.wav");
	SOUNDMANAGER->CreateSound("attack", L"./Sound/bounce.wav");
	SOUNDMANAGER->CreateSound("yjhbs", L"./Sound/yjh.wav");
	SOUNDMANAGER->CreateSound("yjhbs2", L"./Sound/yjh2.wav");
	SOUNDMANAGER->CreateSound("destroy", L"./Sound/droid_destroyed.wav");

	//Background
	MESHMANAGER->AddMesh("Map1", L"./Resource/map/map.obj");
	MESHMANAGER->AddMesh("Map2", L"./Resource/map_2/map_2.obj");
	IMAGEMANAGER->AddTexture("minimap2_1", "./Resource/map_2/minimap/side.png");
	IMAGEMANAGER->AddTexture("minimap2_2", "./Resource/map_2/minimap/top.png");
	
	//Player
	MESHMANAGER->AddAnimeMesh("Player_Idle", L"./Resource/Player/player_idle/player_idle%d.obj", 0, ANIMEFRAME);
	MESHMANAGER->AddAnimeMesh("Player_move", L"./Resource/Player/player_move/player_move%d.obj", 0, ANIMEFRAME);
	MESHMANAGER->AddAnimeMesh("Player_Idle1", L"./Resource/Player/player_idle_1_upgrade/player_idle_1_upgrade%d.obj", 0, ANIMEFRAME);
	MESHMANAGER->AddAnimeMesh("Player_move1", L"./Resource/Player/player_move_1_upgrade/player_move_1_upgrade%d.obj", 0, ANIMEFRAME);
	MESHMANAGER->AddAnimeMesh("Player_Idle2", L"./Resource/Player/player_idle_2_upgrade/player_idle_2_upgrade%d.obj", 0, ANIMEFRAME);
	MESHMANAGER->AddAnimeMesh("Player_move2", L"./Resource/Player/player_move_2_upgrade/player_move_2_upgrade%d.obj", 0, ANIMEFRAME);
	MESHMANAGER->AddAnimeMesh("Player_arm_Idle", L"./Resource/Player/player_arm/player_arm%d.obj", 0, ANIMEFRAME);
	MESHMANAGER->AddAnimeMesh("Player_arm_Idle", L"./Resource/Player/player_arm_attack/player_arm_attack%d.obj", 0, ANIMEFRAME);

	IMAGEMANAGER->AddTexture("minimap1", "./Resource/map/minimap/minimap_side.png");
	IMAGEMANAGER->AddTexture("minimap2", "./Resource/map/minimap/minimap_top.png");

	//Enemy
	MESHMANAGER->AddMesh("AirEnemy1", L"./Resource/Enemy/enemy/enemy_2.obj");
	MESHMANAGER->AddAnimeMesh("Rock", L"./Resource/Enemy/stone/stone%d.obj", 0, 10);
	MESHMANAGER->AddMesh("AirEnemy2", L"./Resource/Enemy/enemy_2/enemy_1.obj");

	//Bullet
	MESHMANAGER->AddMesh("120MM", L"./Resource/Bullet/120MM/skill.obj");
	IMAGEMANAGER->AddTexture("88MM", "./Resource/Bullet/88MM/88MM.png");
	MESHMANAGER->AddAnimeMesh("enemyBullet", L"./Resource/Enemy/enemy_attack/%d.obj", 0, 10);

	//Effect
	MESHMANAGER->AddMesh("Plane", L"./Resource/Effect/Plane/bullet.obj");
	IMAGEMANAGER->AddAnimeTexture("speedUpAir", "./Resource/Effect/speed_up/%d.png", 1, 4);
	IMAGEMANAGER->AddAnimeTexture("./Resource/nuclear_effect/%d.png", "./Resource/nuclear_effect/%d.png", 1, 31);
	MESHMANAGER->AddMesh("speedUpModel", L"./Resource/Effect/speed_up_model/speed_up.obj");
	IMAGEMANAGER->AddAnimeTexture("poison", "./Resource/Effect/poison/poison_%d.png", 1, 9);
	IMAGEMANAGER->AddAnimeTexture("SceneChange", "./Resource/scene/scene_%d.png", 1, 17);

	//SavePoint
	MESHMANAGER->AddMesh("save_point", L"./Resource/save_point/save_point.obj");

	//Item
	MESHMANAGER->AddMesh("doubleJump", L"./Resource/Item/double_jump/double_jump.obj");

	//UI
	IMAGEMANAGER->AddTexture("EndingScore", "./Resource/UI/Score.png");
	IMAGEMANAGER->AddTexture("EndingName", "./Resource/UI/Name.png");
	IMAGEMANAGER->AddAnimeTexture("HappyEnding", "./Resource/ending/happy_ending_%d.png", 1, 2);
	isEndThread = true;
}

void LoadScene::Update()
{
	if (isEndThread)
	{
		if (meshThread.joinable()) {
			meshThread.join();
			SCENEMANAGER->AddScene(new MainScene());
		}
	}
}

void LoadScene::Render()
{
	frame += 0.015;
	int count = int(frame * 12) % animeTexture.size();
	IMAGEMANAGER->DrawTexture(animeTexture[count], {640, 360});
}

void LoadScene::Release()
{
}

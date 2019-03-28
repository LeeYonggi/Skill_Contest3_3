#include "DXUT.h"
#include "MainScene.h"

#include "UI.h"
#include "IngameScene.h"
#include <fstream>

MainScene::MainScene()
{
}


MainScene::~MainScene()
{
}

void MainScene::Init()
{
	soundKey = SOUNDMANAGER->CreateSound("MainGame", L"./Sound/Electro_1.wav");
	SOUNDMANAGER->PlayCSound(soundKey, true);
	DATAMANAGER->point = 0;
	vUI.push_back(OBJECTMANAGER->AddObject(OBJ_UIEFFECT, new UI({640, 360}, "Intro", 0, 50, false, 16)));
	vUI.push_back(OBJECTMANAGER->AddObject(OBJ_UI, new UI(true, { 640, 400 }, {328, 91},
		"./Resource/UI/main/start_no_click.png", "./Resource/UI/main/start_click.png", []() { SCENEMANAGER->AddScene(new IngameScene(STAGE_1)); })));
	vUI.push_back(OBJECTMANAGER->AddObject(OBJ_UI, new UI(true, { 640, 520 }, { 328, 91 },
		"./Resource/UI/main/infor_no_click.png", "./Resource/UI/main/infor_click.png", [=]() {this->isInfo = true; })));
	vUI.push_back(OBJECTMANAGER->AddObject(OBJ_UI, new UI(true, { 640, 640 }, { 328, 91 },
		"./Resource/UI/main/how_no_click.png", "./Resource/UI/main/how_click.png", [=]() {this->isHowto = true; })));
	vUI.push_back(OBJECTMANAGER->AddObject(OBJ_UI, new UI(true, { 1206, 645 }, { 92, 91 },
		"./Resource/UI/main/ranking_no_click.png", "./Resource/UI/main/ranking_click.png", [=]() {this->isRank = true; })));

	for (int i = 1; i < 5; i++)
		vUI[i]->isActive = false;
	info = IMAGEMANAGER->AddTexture("./Resource/UI/info.png", "./Resource/UI/info.png");
	howto = IMAGEMANAGER->AddTexture("./Resource/UI/how.png", "./Resource/UI/how.png");
	rank = IMAGEMANAGER->AddTexture("./Resource/UI/ranking.png", "./Resource/UI/ranking.png");
	title = IMAGEMANAGER->AddTexture("./Resource/UI/main/title.png", "./Resource/UI/main/title.png");
}

void MainScene::Update()
{
	frame += ELTIME;
}

bool operator <(const Acount &a1, const Acount &a2)
{
	return (a1.point > a2.point);
}
void MainScene::Render()
{
	if (vUI[0]->animeEnd)
	{
		endIntro = true;
		for (int i = 0; i < 5; i++)
			vUI[i]->isActive = true;
	}
	if (!endIntro) return;
	IMAGEMANAGER->DrawTexture(title, { 640, 360 });
	if (isInfo)
	{
		IMAGEMANAGER->DrawTexture(info, {640, 360});
	}
	if (isHowto)
	{
		IMAGEMANAGER->DrawTexture(howto, { 640, 360 });
	}
	if (isRank)
	{
		IMAGEMANAGER->DrawTexture(rank, { 640, 360 });
		fstream fs;
		vector<Acount> vAcount;
		fs.open("./Data/Rank.txt");
		while (!fs.eof())
		{
			string name;
			int point;
			fs >> name >> point;
			vAcount.push_back({name, point});
		}
		auto iter = vAcount.end() - 1;
		vAcount.erase(iter);
		std::sort(vAcount.begin(), vAcount.end());
		for(int i = 0; i < vAcount.size(); i++)
			IMAGEMANAGER->DrawFont(vAcount[i].name + "  " + to_string(vAcount[i].point), {300, 100 + i * 203.0f }, 100);
		fs.close();
	}
	if (INPUTMANAGER->KeyDown(VK_LBUTTON))
	{
		isRank = false;
		isInfo = false;
		isHowto = false;
	}
}

void MainScene::Release()
{
	SOUNDMANAGER->StopSound(soundKey);
}

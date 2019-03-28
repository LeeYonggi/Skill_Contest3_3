#include "DXUT.h"
#include "EndingScene.h"

#include "MainScene.h"
#include <fstream>

EndingScene::EndingScene(bool _isHappy)
{
	isHappy = _isHappy;
}

EndingScene::~EndingScene()
{
	animeTex.clear();
}

void EndingScene::Init()
{
	frame = 0;
	if (isHappy)
	{
		animeTex = IMAGEMANAGER->AddAnimeTexture("HappyEnding", "./Resource/ending/happy_ending_%d.png", 1, 2);
		score = IMAGEMANAGER->AddTexture("EndingScore", "./Resource/UI/Score.png");
		nameTex = IMAGEMANAGER->AddTexture("EndingName", "./Resource/UI/Name.png");
	}
	else
	{
		texture = IMAGEMANAGER->AddTexture("BadEnding", "./Resource/ending/bad_ending.png");
		animeTex = IMAGEMANAGER->AddAnimeTexture("HappyEnding", "./Resource/game_over/%d.png", 1, 6);
	}
	name = "";
}

void EndingScene::Update()
{
	if (isHappy)
	{
		bool isEnter = false;
		for (int i = 0; i < 256; i++)
		{
			if (INPUTMANAGER->KeyDown(VK_RETURN))
			{
				isEnter = true;
				SCENEMANAGER->AddScene(new MainScene());
			}
			else if (INPUTMANAGER->KeyDown(i) && INPUTMANAGER->KeyDown(VK_F3) == false)
				name.push_back(i);
		}
		if (isEnter)
		{
			fstream fs;
			fs.open("./Data/Rank.txt", ios::app);
			fs << name << "\t" << DATAMANAGER->point << endl;
			fs.close();
		}
	}
	else
	{
		if (INPUTMANAGER->KeyDown(VK_LBUTTON))
		{
			SCENEMANAGER->AddScene(new MainScene());
		}
	}
	frame += ELTIME;
}

void EndingScene::Render()
{
	if (isHappy)
	{
		int count = int(frame * 5) % animeTex.size();
		IMAGEMANAGER->DrawTexture(animeTex[count], { 640, 360 }, 0, { 1, 1 }, { 1, 1 }, {1, 1, 1, frame * 0.4f });
		IMAGEMANAGER->DrawTexture(score, {192, 230}, 0, { 1, 1 }, { 1, 1 }, { 1, 1, 1, frame * 0.4f });
		IMAGEMANAGER->DrawTexture(nameTex, { 192, 330 }, 0, { 1, 1 }, { 1, 1 }, { 1, 1, 1, frame * 0.4f });
		IMAGEMANAGER->DrawFont(to_string(DATAMANAGER->point), { 130, 180}, 100);
		IMAGEMANAGER->DrawFont(name, { 130, 280}, 100);
	}
	else
	{
		IMAGEMANAGER->DrawTexture(texture, {640, 360}, 0, { 1, 1 }, { 1, 1 }, { 1, 1, 1, frame * 0.4f });
		if (frame * 5 < animeTex.size())
		{
			int count = int(frame * 5) % animeTex.size();
			IMAGEMANAGER->DrawTexture(animeTex[count], { 640, 360 });
		}
	}
}

void EndingScene::Release()
{
}

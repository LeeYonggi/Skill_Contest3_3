#include "DXUT.h"
#include "Effect.h"

#include "Camera.h"


Effect::Effect(string route, int low, int high, Vector3 _pos, Vector3 _moveVector, float _scale, bool _isLoop, float _dieTime)
{
	animeTexture.push_back(IMAGEMANAGER->AddAnimeTexture(route, route, low, high));
	mesh = MESHMANAGER->AddMesh("Plane", L"./Resource/Effect/Plane/bullet.obj");
	moveVector = _moveVector;
	scale = { _scale, _scale, _scale };
	isLoop = _isLoop;
	dieTime = _dieTime;
	pos = _pos;
}

Effect::Effect(string route, int low, int high, Vector3 _pos, Vector3 _moveVector, float _scale)
{
	animeTexture.push_back(IMAGEMANAGER->AddAnimeTexture(route, route, low, high));
	mesh = MESHMANAGER->AddMesh("Plane", L"./Resource/Effect/Plane/bullet.obj");
	moveVector = _moveVector;
	scale = { _scale, _scale, _scale };
	isLoop = false;
	dieTime = 0.0f;
	pos = _pos;
}

Effect::~Effect()
{
}

void Effect::Init()
{
}

void Effect::Update()
{
	pos += moveVector * ELTIME;
	frame += ELTIME;
	if (isLoop)
	{
		if (frame > dieTime)
			isDestroy = true;
	}
	else if (int(frame * 12) >= animeTexture[0].size())
	{
		isActive = false;
		isDestroy = true;
	}
}

void Effect::Render()
{
	matR = GetBillboard(pos, CAMERAMANAGER->mainCamera->eye);
	float count = int(frame * 12) % animeTexture[0].size();
	MESHMANAGER->DrawEffect(mesh, animeTexture[0][count], pos, matR, scale);
}

void Effect::Release()
{
}

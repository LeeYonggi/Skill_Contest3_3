#include "DXUT.h"
#include "TsCamera.h"

#include "Player.h"


TsCamera::TsCamera(Player * _player)
{
	player = _player;
}

TsCamera::~TsCamera()
{
}

void TsCamera::Init()
{
}

void TsCamera::Update()
{
	Vector3 temp = Vector3(player->pos.x, player->pos.y + 15, player->pos.z - 20);
	eye = d3d::Lerp(eye, temp, 0.1f);
	at = player->pos;
	Camera::Update();
}


void TsCamera::Release()
{
}

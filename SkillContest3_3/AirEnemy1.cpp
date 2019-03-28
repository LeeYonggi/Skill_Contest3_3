#include "DXUT.h"
#include "AirEnemy1.h"


AirEnemy1::AirEnemy1(Player *player, Vector3 _pos)
	: Enemy(player, _pos)
{
	radius = 4.0f;
}


AirEnemy1::~AirEnemy1()
{
}

void AirEnemy1::Init()
{
	mesh = MESHMANAGER->AddMesh("AirEnemy1", L"./Resource/Enemy/enemy/enemy_2.obj");
	scale = { 0.03f, 0.03f, 0.03f };
	speed = 1.0f;
}

void AirEnemy1::Update()
{
	if (!IsInScreen()) return;
	AirEnemyMove();
	EnemyAttacked();
	AirEnemyAttack();
}

void AirEnemy1::Render()
{
	RotateLerp({0, 0, 0}, moveVector, &matR);
	Matrix mat;
	D3DXMatrixRotationY(&mat, D3DXToRadian(-90));
	matR *= mat;
	MESHMANAGER->DrawMesh(mesh, pos, matR, scale);
}

void AirEnemy1::Release()
{
}

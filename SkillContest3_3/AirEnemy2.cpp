#include "DXUT.h"
#include "AirEnemy2.h"


AirEnemy2::AirEnemy2(Player *player, Vector3 _pos)
	: Enemy(player, _pos)
{
	radius = 4.0f;
	hp = 700;
}

AirEnemy2::~AirEnemy2()
{
}

void AirEnemy2::Init()
{
	mesh = MESHMANAGER->AddMesh("AirEnemy2", L"./Resource/Enemy/enemy_2/enemy_1.obj");
	scale = { 0.05f, 0.05f, 0.05f };
	speed = 1.0f;
}

void AirEnemy2::Update()
{
	if (!IsInScreen()) return;
	AirEnemyMove();
	EnemyAttacked();
	AirEnemyAttack();
}

void AirEnemy2::Render()
{
	RotateLerp({ 0, 0, 0 }, moveVector, &matR);
	Matrix mat;
	D3DXMatrixRotationY(&mat, D3DXToRadian(-90));
	matR *= mat;
	MESHMANAGER->DrawMesh(mesh, pos, matR, scale);
}

void AirEnemy2::Release()
{
}

#include "DXUT.h"
#include "Rock.h"

#include "Item.h"

Rock::Rock(Player *player, Vector3 _pos)
	:Enemy(player, _pos)
{
	radius = 10;
}


Rock::~Rock()
{
}

void Rock::Init()
{
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Rock", L"./Resource/Enemy/stone/stone%d.obj", 0, 10));
	scale = { 0.3, 0.3, 0.3 };

	hp = 1200;
}

void Rock::Update()
{
	if (!IsInScreen()) return;
	if (EnemyAttacked())
	{
		frame += ELTIME * 12;
		if (frame >= animeMesh[0].size())
		{
			isActive = false;
			isDestroy = true;
			int number = d3d::GetRandomNumber(0, 5);
			OBJECTMANAGER->AddObject(OBJ_KINDS::OBJ_ITEM, new Item(pos, (ITEM_KIND)number));
		}
	}
}

void Rock::Render()
{
	MESHMANAGER->DrawMesh(animeMesh[0][(int)frame], { pos.x, pos.y - 2.5f, pos.z }, matR, scale);
}

void Rock::Release()
{
}

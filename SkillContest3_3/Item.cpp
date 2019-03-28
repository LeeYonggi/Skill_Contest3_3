#include "DXUT.h"
#include "Item.h"


Item::Item(Vector3 _pos, ITEM_KIND kind)
{
	pos = _pos;
	itemKind = kind;
	scale = { 0.05, 0.05, 0.05 };
	radius = 2;
}


Item::~Item()
{
}

void Item::Init()
{
	switch (itemKind)
	{
	case ITEM_JUMP:
		mesh = MESHMANAGER->AddMesh("doubleJump", L"./Resource/Item/double_jump/double_jump.obj");
		break;
	case ITEM_SPEEDUP:
		mesh = MESHMANAGER->AddMesh("speed_up", L"./Resource/Item/speed_up/speed_up.obj");
		break;
	case ITEM_LENGTHUP:
		mesh = MESHMANAGER->AddMesh("range_up", L"./Resource/Item/range_up/range_up.obj");
		break;
	case ITEM_THIRD:
		mesh = MESHMANAGER->AddMesh("3_direction", L"./Resource/Item/3_direction/3_direction.obj");
		break;
	case ITEM_TRAKINGMISSILE:
		mesh = MESHMANAGER->AddMesh("missile", L"./Resource/Item/missile/missile.obj");
		break;
	case ITEM_NUCLEAR:
		mesh = MESHMANAGER->AddMesh("nuclear", L"./Resource/Item/nuclear/nuclear.obj");
		break;
	default:
		break;
	}
}

void Item::Update()
{
	pos.y += sin(frame * 5) * 0.2;
	frame += ELTIME;
}

void Item::Render()
{
	MESHMANAGER->DrawAlphaMesh(mesh, pos, matR, scale);
}

void Item::Release()
{
}

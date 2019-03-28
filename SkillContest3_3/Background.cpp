#include "DXUT.h"
#include "Background.h"


Background::Background(STAGE_STATE _stage)
{
	stage = _stage;
}


Background::~Background()
{
}

void Background::Init()
{
	switch (stage)
	{
	case STAGE_1:
		mesh = MESHMANAGER->AddMesh("Map1", L"./Resource/map/map.obj");
		minimap1 = IMAGEMANAGER->AddTexture("minimap1", "./Resource/map/minimap/minimap_side.png");
		minimap2 = IMAGEMANAGER->AddTexture("minimap2", "./Resource/map/minimap/minimap_top.png");
		pos = { 0, 0, -12 };
		break;
	case STAGE_2:
		mesh = MESHMANAGER->AddMesh("Map2", L"./Resource/map_2/map_2.obj");
		minimap1 = IMAGEMANAGER->AddTexture("minimap2_1", "./Resource/map_2/minimap/side.png");
		minimap2 = IMAGEMANAGER->AddTexture("minimap2_2", "./Resource/map_2/minimap/top.png");
		pos = { 0, 0, -14 };
		break;
	default:
		break;
	}

}

void Background::Update()
{
}

void Background::Render()
{
	MESHMANAGER->DrawMesh(mesh, pos, matR, scale);
}

void Background::Release()
{
}

#pragma once
#include "Singleton.h"

struct Texture
{
	D3DXIMAGE_INFO info;
	LPDIRECT3DTEXTURE9 tex;
};
class ImageManager :
	public Singleton<ImageManager>
{
public:
	ImageManager();
	virtual ~ImageManager();

private:
	map<string, Texture*> m_Texture;
	LPD3DXSPRITE sprite;

public:
	Texture *AddTexture(string str, string file);
	vector<Texture*> AddAnimeTexture(string str, string route, int low, int high);
	void DrawTexture(string str, Vector2 pos, float rotate = 0.0f, Vector2 scale = { 1.0f, 1.0f },
		Vector2 length = { 1.0f, 1.0f},	D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	void DrawTexture(Texture *texture, Vector2 pos, float rotate = 0.0f, Vector2 scale = { 1.0f, 1.0f },
		Vector2 length = { 1.0f, 1.0f }, D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	void DrawFont(string str, Vector2 pos, float size);
};

#define IMAGEMANAGER ImageManager::GetInstance()
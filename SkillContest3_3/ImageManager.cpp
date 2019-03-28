#include "DXUT.h"
#include "ImageManager.h"


ImageManager::ImageManager()
{
	D3DXCreateSprite(DEVICE, &sprite);

}


ImageManager::~ImageManager()
{
	for (auto iter : m_Texture)
	{
		SAFE_RELEASE(iter.second->tex);
		SAFE_DELETE(iter.second);
	}
	m_Texture.clear();

	SAFE_RELEASE(sprite);
}

Texture * ImageManager::AddTexture(string str, string file)
{
	auto iter = m_Texture.find(str);
	if (iter != m_Texture.end()) return iter->second;

	Texture *temp = new Texture;

	D3DXCreateTextureFromFileExA(DEVICE,
		file.c_str(),
		D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,
		0, 0,
		D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT,
		0, &temp->info,
		0,
		&temp->tex);

	m_Texture.insert(make_pair(str, temp));

	return temp;
}

vector<Texture*> ImageManager::AddAnimeTexture(string str, string route, int low, int high)
{
	vector<Texture*> temp;
	for (int i = low; i <= high; i++)
	{
		CHAR ctr[128];
		sprintf(ctr, route.c_str(), i);
		temp.push_back(AddTexture(str + to_string(i), ctr));
	}
	return temp;
}

void ImageManager::DrawTexture(string str, Vector2 pos, float rotate, Vector2 scale, Vector2 length, D3DXCOLOR color)
{
	auto iter = m_Texture.find(str);
	if (iter == m_Texture.end()) return;

	Matrix matW, matR, matT, matS;

	D3DXMatrixScaling(&matS, scale.x, scale.y, 1);
	D3DXMatrixRotationZ(&matR, rotate);
	D3DXMatrixTranslation(&matT, pos.x, pos.y, 0);

	matW = matS * matR * matT;
	
	sprite->SetTransform(&matW);

	Vector3 center = { (float)iter->second->info.Width * 0.5f, (float)iter->second->info.Height * 0.5f, 0 };

	RECT rect;
	rect = {0, 0, (long)(iter->second->info.Width * length.x), (long)(iter->second->info.Height * length.y) };

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(iter->second->tex, &rect, &center, nullptr, color);
	sprite->End();
}

void ImageManager::DrawTexture(Texture * texture, Vector2 pos, float rotate, Vector2 scale, Vector2 length, D3DXCOLOR color)
{
	Matrix matW, matR, matT, matS;

	D3DXMatrixScaling(&matS, scale.x, scale.y, 1);
	D3DXMatrixRotationZ(&matR, rotate);
	D3DXMatrixTranslation(&matT, pos.x, pos.y, 0);

	matW = matS * matR * matT;

	sprite->SetTransform(&matW);

	Vector3 center = { (float)texture->info.Width * 0.5f, (float)texture->info.Height * 0.5f, 0 };

	RECT rect;
	rect = { 0, 0, (long)(texture->info.Width * length.x), (long)(texture->info.Height * length.y) };

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(texture->tex, &rect, &center, nullptr, color);
	sprite->End();
}

void ImageManager::DrawFont(string str, Vector2 pos, float size)
{
	LPD3DXFONT font;
	D3DXCreateFontA(DEVICE, size, 0, FW_HEAVY, 1, false, HANGUL_CHARSET, 0, 0, 0, "¸¼Àº °íµñ", &font);

	Matrix mat;
	D3DXMatrixTranslation(&mat, pos.x, pos.y, 0);
	sprite->SetTransform(&mat);

	RECT re = { 0 };

	sprite->Begin(D3DXSPRITE_ALPHABLEND);

	font->DrawTextA(sprite, str.c_str(), -1, &re, DT_NOCLIP, 0XFFFFFFFF);

	sprite->End();

	SAFE_RELEASE(font);
}

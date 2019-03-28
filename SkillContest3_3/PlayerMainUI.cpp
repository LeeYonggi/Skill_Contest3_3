#include "DXUT.h"
#include "PlayerMainUI.h"

#include "Player.h"
#include "UI.h"


PlayerMainUI::PlayerMainUI(Player *_player, Vector2 _pos, string str, int low, int high)
	: UI(_pos, str, low, high, true, 12)
{
	player = _player;
}


PlayerMainUI::~PlayerMainUI()
{
}

void PlayerMainUI::Init()
{
	UI::Init();
}

void PlayerMainUI::Update()
{
	UI::Update();
}

void PlayerMainUI::Render()
{
	UI::Render();
	for (int i = 0; i < 6; i++)
		IMAGEMANAGER->DrawFont(to_string(player->itemCount[i]), { 115 + (float)127 * i, 660 }, 50);
	IMAGEMANAGER->DrawFont(to_string(DATAMANAGER->point), { 1000, 0 }, 100);
	IMAGEMANAGER->DrawFont(to_string(player->totalHp), { 180, 0 }, 100);
	IMAGEMANAGER->DrawFont(to_string(player->upgrade + 1), { 1150, 610 }, 100);
}

void PlayerMainUI::Release()
{
	UI::Release();
}

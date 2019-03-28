#pragma once
#include "UI.h"

class Player;
class PlayerMainUI :
	public UI
{
public:
	PlayerMainUI(Player *_player, Vector2 _pos, string str, int low, int high);
	virtual ~PlayerMainUI();

private:
	Player *player;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
};


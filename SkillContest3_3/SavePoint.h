#pragma once
#include "Object.h"

class Player;
class SavePoint :
	public Object
{
public:
	SavePoint(Player *_player, Vector3 _pos, STAGE_STATE _stage, float _isLast);
	virtual ~SavePoint();

private:
	Player *player;
	bool isLast;
	STAGE_STATE stage;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;

public:

};


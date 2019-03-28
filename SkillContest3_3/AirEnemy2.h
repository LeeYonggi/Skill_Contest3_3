#pragma once
#include "Enemy.h"
class AirEnemy2 :
	public Enemy
{
public:
	AirEnemy2(Player *player, Vector3 _pos);
	virtual ~AirEnemy2();

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
};


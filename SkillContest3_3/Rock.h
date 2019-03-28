#pragma once
#include "Enemy.h"
class Rock :
	public Enemy
{
public:
	Rock(Player *player, Vector3 _pos);
	virtual ~Rock();

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
};


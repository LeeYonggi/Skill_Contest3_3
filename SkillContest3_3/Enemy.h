#pragma once
#include "Object.h"

#include "CharacterAttack.h"

class Player;
class Enemy :
	public Object
{
public:
	Enemy(Player *_player, Vector3 _pos);
	virtual ~Enemy();

protected:
	Player *player;
	Vector3 velocity;
	float speed;

public:
	int hp;
	CharacterAttack attack;


public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;

public:
	void AirEnemyMove();
	bool EnemyAttacked();
	bool IsInScreen();
	void AirEnemyAttack();
};


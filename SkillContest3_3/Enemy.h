#pragma once
#include "Object.h"

#include "CharacterAttack.h"

enum ENEMY_STATE
{
	ENEMY_MOVE,
	ENEMY_ATTACK
};
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
	ENEMY_STATE state = ENEMY_MOVE;
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
	void EnemyStateMove();
};


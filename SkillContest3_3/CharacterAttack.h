#pragma once


enum BULLET_STATE;
class CharacterAttack
{
public:
	CharacterAttack();
	virtual ~CharacterAttack();

public:
	bool isAttack;
	float frame;
	float attackDelay;

public:
	void AttackUpdate();
	void Attack(OBJ_KINDS obj, BULLET_STATE state, Vector3 pos, Vector3 moveVector, float gravity);
	void Attack(OBJ_KINDS obj, BULLET_STATE state, Vector3 pos, Vector3 moveVector, float gravity, Object *target);
};


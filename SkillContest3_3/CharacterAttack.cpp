#include "DXUT.h"
#include "CharacterAttack.h"

#include "Bullet.h"


CharacterAttack::CharacterAttack()
{
	frame = 0;
	isAttack = false;
	attackDelay = 1.0f;
}


CharacterAttack::~CharacterAttack()
{
}

void CharacterAttack::AttackUpdate()
{
	if (isAttack)
	{
		frame += ELTIME;
		if (frame > attackDelay)
		{
			frame = 0;
			isAttack = false;
		}
	}
}

void CharacterAttack::Attack(OBJ_KINDS obj, BULLET_STATE state, Vector3 pos, Vector3 moveVector, float gravity)
{
	if (isAttack) return;
	isAttack = true;
	OBJECTMANAGER->AddObject(obj, new Bullet(state, pos, moveVector, gravity));
}

void CharacterAttack::Attack(OBJ_KINDS obj, BULLET_STATE state, Vector3 pos, Vector3 moveVector, float gravity, Object * target)
{
	if (isAttack) return;
	isAttack = true;
	OBJECTMANAGER->AddObject(obj, new Bullet(state, pos, moveVector, gravity, target));
}

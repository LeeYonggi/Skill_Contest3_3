#pragma once
#include "Object.h"

enum BULLET_STATE
{
	BULLET_120MM,
	BULLET_88MM,
	BULLET_TARGET,
	BULLET_ENEMY
};

enum BULLET_FUNC_STATE
{
	BULLET_INIT,
	BULLET_UPDATE,
	BULLET_RENDER,
	BULLET_RELEASE
};
class Bullet :
	public Object
{
	using BulletFunc = void(Bullet::*)(BULLET_FUNC_STATE);
public:
	Bullet(BULLET_STATE _state, Vector3 _pos, Vector3 _moveVector, float _gravity);
	Bullet(BULLET_STATE _state, Vector3 _pos, Vector3 _moveVector, float _gravity, Object *_target);
	virtual ~Bullet();

private:
	BulletFunc bulletFunc[BULLET_ENEMY + 1];
	BULLET_STATE state;
	Object *target;
	float speed;
	float gravity;
	float velocity;

public:
	int damage;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;

public:
	void Bullet120(BULLET_FUNC_STATE state);
	void Bullet88(BULLET_FUNC_STATE state);
	void BulletTarget(BULLET_FUNC_STATE state);
	void BulletEnemy(BULLET_FUNC_STATE state);
};


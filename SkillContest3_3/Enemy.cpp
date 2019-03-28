#include "DXUT.h"
#include "Enemy.h"

#include "Player.h"
#include "Bullet.h"
#include "Particle.h"
#include "Background.h"
#include "Effect.h"


Enemy::Enemy(Player *_player, Vector3 _pos)
{
	player = _player;
	velocity = { 0, 0, 0 };
	pos = _pos;
	speed = 1.0f;
	hp = 500;
	vParticle.push_back(OBJECTMANAGER->AddObject(OBJ_EFFECT, new Particle("./Resource/Effect/bomb/bomb_%d.png", 1, 8, 0.2f, 0.7f)));
	
}


Enemy::~Enemy()
{
}

void Enemy::Init()
{
}

void Enemy::Update()
{

}

void Enemy::Render()
{
}

void Enemy::Release()
{
}

void Enemy::AirEnemyMove()
{
	if (hp < 1)
	{
		velocity.y -= 1;
	}
	else
	{
		moveVector = player->pos;
		moveVector.y += 9;
		moveVector = GetTargetVec3(pos, moveVector);

		velocity += moveVector;
		velocity.x = max(min(velocity.x, 20), -20);
		velocity.y = max(min(velocity.y, 3), -3);
		velocity.z = max(min(velocity.z, 20), -20);
	}
	Vector2 temp;
	temp.x = pos.x;
	temp.y = pos.y;
	auto iter = OBJECTMANAGER->GetObjectList(OBJ_BACKGROUND);
	auto obj = *iter->begin();
	Background *back = dynamic_cast<Background*>(obj);
	if (GetPixelCollision(temp * 10, back->minimap1))
	{
		OBJECTMANAGER->AddObject(OBJ_EFFECT, 
			new Effect("./Resource/Effect/bomb/bomb_%d.png", 1, 8, pos, { 0, 0, 0 }, 0.3f));
		isDestroy = true;
		CAMERAMANAGER->CameraShake(0.5f);
		SOUNDMANAGER->DuplicatePlay("destroy");
		DATAMANAGER->point += 100;
	}
	
	pos += velocity * ELTIME * speed;
}

bool Enemy::EnemyAttacked()
{
	bool isDie = false;
	if (hp < 1)
	{
		vParticle[0]->isActive = true;
		float number = d3d::GetRandomNumber(10, 20);
		vParticle[0]->ParticleInit(pos, -moveVector * 4, number * 0.01f, false);

		isDie = true;
	}
	else
	{
		auto iter = OBJECTMANAGER->GetObjectList(OBJ_PBULLET);
		for (auto _iter = iter->begin(); _iter != iter->end(); ++_iter)
		{
			if (GetCircleCollision(pos, (*_iter)->pos, radius, (*_iter)->radius))
			{
				Bullet *bullet = (Bullet*)((*_iter));
				hp -= bullet->damage;
				bullet->isDestroy = true;
			}
		}
	}
	

	return isDie;
}

bool Enemy::IsInScreen()
{
	if (pos.x < CAMERAMANAGER->mainCamera->eye.x + 40 &&
		pos.x > CAMERAMANAGER->mainCamera->eye.x - 40)
		return true;
	return false;
}

void Enemy::AirEnemyAttack()
{
	attack.AttackUpdate();
	attack.attackDelay = 2.0f;
	Vector3 temp = GetTargetVec3(pos, player->pos);
	attack.Attack(OBJ_EBULLET, BULLET_ENEMY, pos, temp, 0);
}

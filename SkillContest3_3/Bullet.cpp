#include "DXUT.h"
#include "Bullet.h"

#include "Background.h"
#include "Camera.h"

#include "Particle.h"
#include "Effect.h"
#include "Enemy.h"

Bullet::Bullet(BULLET_STATE _state, Vector3 _pos, Vector3 _moveVector, float _gravity)
{
	state = _state;
	pos = _pos;
	moveVector = _moveVector;
	gravity = _gravity;
	velocity = 0;
	speed = 1;
	target = nullptr;
}

Bullet::Bullet(BULLET_STATE _state, Vector3 _pos, Vector3 _moveVector, float _gravity, Object * _target)
{
	state = _state;
	pos = _pos;
	moveVector = _moveVector;
	gravity = _gravity;
	velocity = 0;
	speed = 1;
	target = _target;
}


Bullet::~Bullet()
{
}

void Bullet::Init()
{
	bulletFunc[BULLET_STATE::BULLET_120MM] = &Bullet::Bullet120;
	bulletFunc[BULLET_STATE::BULLET_88MM] = &Bullet::Bullet88;
	bulletFunc[BULLET_STATE::BULLET_TARGET] = &Bullet::BulletTarget;
	bulletFunc[BULLET_STATE::BULLET_ENEMY] = &Bullet::BulletEnemy;

	(this->*bulletFunc[state])(BULLET_FUNC_STATE::BULLET_INIT);
}

void Bullet::Update()
{
	(this->*bulletFunc[state])(BULLET_FUNC_STATE::BULLET_UPDATE);
	auto iter = OBJECTMANAGER->GetObjectList(OBJ_BACKGROUND);
	Background *back = dynamic_cast<Background*>(*iter->begin());

	Vector2 groundCheck;
	groundCheck.x = pos.x;
	groundCheck.y = pos.y;
	if (GetPixelCollision(groundCheck * 10, back->minimap1) ||
		pos.x < -100 || pos.x > 1000 ||
		pos.y < -100 || pos.y > 100 ||
		pos.z < -500 || pos.z > 500)
	{
		isDestroy = true;
	}
	frame += ELTIME;
}

void Bullet::Render()
{
	(this->*bulletFunc[state])(BULLET_FUNC_STATE::BULLET_RENDER);
}

void Bullet::Release()
{
	(this->*bulletFunc[state])(BULLET_FUNC_STATE::BULLET_RELEASE);
}

void Bullet::Bullet120(BULLET_FUNC_STATE state)
{
	Vector3 pastVec3 = pos;
	Vector3 distance;
	float pScale;
	switch (state)
	{
	case BULLET_FUNC_STATE::BULLET_INIT:
		mesh = MESHMANAGER->AddMesh("120MM", L"./Resource/Bullet/120MM/skill.obj");
		scale = { 0.1, 0.1, 0.1 };
		velocity = 0.8f;
		speed = 50.0f;
		vParticle.push_back(OBJECTMANAGER->AddObject(OBJ_EFFECT, 
			new Particle("./Resource/Effect/dust/dust_%d.png", 1, 5, 0.1f, 0.6f)));
		damage = 300;
		break;
	case BULLET_FUNC_STATE::BULLET_UPDATE:
		velocity -= gravity;
		pos.y += velocity;
		pos += moveVector * ELTIME * speed;
		distance = pos - pastVec3;

		D3DXVec3Normalize(&distance, &distance);
		D3DXMatrixLookAtLH(&matR, &Vector3(0, 0, 0), &distance, &Vector3(0, 1, 0));
		D3DXMatrixTranspose(&matR, &matR);

		pScale = d3d::GetRandomNumber(40, 80);
		vParticle[0]->isActive = true;
		vParticle[0]->ParticleInit(pos, -distance * 10, pScale * 0.001f, false);

		break;
	case BULLET_FUNC_STATE::BULLET_RENDER:
		MESHMANAGER->DrawMesh(mesh, pos, matR, scale);
		break;
	case BULLET_FUNC_STATE::BULLET_RELEASE:
		OBJECTMANAGER->AddObject(OBJ_EFFECT, 
			new Effect("./Resource/Effect/bombGround/bomb_2_%d.png", 1, 7, pos, { 0, 0, 0 }, 0.3f));

		for (int i = 0; i < 360; i += 30)
		{
			Vector3 bulletVector = { 1, 0, 0 };
			Matrix mat;
			D3DXMatrixRotationZ(&mat, D3DXToRadian(i));
			D3DXVec3TransformCoord(&bulletVector, &bulletVector, &mat);
			D3DXVec3Normalize(&bulletVector, &bulletVector);
			OBJECTMANAGER->AddObject(OBJ_EFFECT,
				new Effect("./Resource/Effect/dust/dust_%d.png", 1, 5, pos, bulletVector * 20, 0.1f));
		}
		break;
	default:
		break;
	}
}

void Bullet::Bullet88(BULLET_FUNC_STATE state)
{
	switch (state)
	{
	case BULLET_FUNC_STATE::BULLET_INIT:
		texture = IMAGEMANAGER->AddTexture("88MM", "./Resource/Bullet/88MM/88MM.png");
		mesh = MESHMANAGER->AddMesh("Plane", L"./Resource/Effect/Plane/bullet.obj");
		speed = 50;
		scale = { 0.02f, 0.02f, 0.02f };
		velocity = 0;
		gravity = 0;
		damage = 100;
		break;
	case BULLET_FUNC_STATE::BULLET_UPDATE:
		pos += moveVector * speed * ELTIME;

		break;
	case BULLET_FUNC_STATE::BULLET_RENDER:

		matR = GetBillboard(pos, CAMERAMANAGER->mainCamera->eye);
		MESHMANAGER->DrawEffect(mesh, texture, pos, matR, scale);
		break;
	case BULLET_FUNC_STATE::BULLET_RELEASE:

		OBJECTMANAGER->AddObject(OBJ_EFFECT,
			new Effect("./Resource/Effect/attack_effect/%d.png", 1, 5, pos, { 0, 0, 0 }, 0.1f));
		break;
	default:
		break;
	}
}

void Bullet::BulletTarget(BULLET_FUNC_STATE state)
{
	Vector3 pastVec3 = pos;
	Vector3 distance;
	float pScale;
	switch (state)
	{
	case BULLET_FUNC_STATE::BULLET_INIT:
		mesh = MESHMANAGER->AddMesh("120MM", L"./Resource/Bullet/120MM/skill.obj");
		scale = { 0.1, 0.1, 0.1 };
		speed = 50.0f;
		vParticle.push_back(OBJECTMANAGER->AddObject(OBJ_EFFECT,
			new Particle("./Resource/Effect/dust/dust_%d.png", 1, 5, 0.1f, 0.6f)));
		damage = 300;
		break;
	case BULLET_FUNC_STATE::BULLET_UPDATE:
		if (target)
		{
			moveVector = GetTargetVec3(pos, target->pos);
			if (reinterpret_cast<Enemy*>(target)->hp < 1)
			{
				isDestroy = true;
			}
		}
		else
			isDestroy = true;
		pos += moveVector * ELTIME * speed;
		distance = pos - pastVec3;

		D3DXVec3Normalize(&distance, &distance);
		D3DXMatrixLookAtLH(&matR, &Vector3(0, 0, 0), &distance, &Vector3(0, 1, 0));
		D3DXMatrixTranspose(&matR, &matR);

		pScale = d3d::GetRandomNumber(40, 80);
		vParticle[0]->isActive = true;
		vParticle[0]->ParticleInit(pos, -distance * 10, pScale * 0.001f, false);

		break;
	case BULLET_FUNC_STATE::BULLET_RENDER:
		MESHMANAGER->DrawMesh(mesh, pos, matR, scale);
		break;
	case BULLET_FUNC_STATE::BULLET_RELEASE:
		OBJECTMANAGER->AddObject(OBJ_EFFECT,
			new Effect("./Resource/Effect/bombGround/bomb_2_%d.png", 1, 7, pos, { 0, 0, 0 }, 0.3f));

		for (int i = 0; i < 360; i += 30)
		{
			Vector3 bulletVector = { 1, 0, 0 };
			Matrix mat;
			D3DXMatrixRotationZ(&mat, D3DXToRadian(i));
			D3DXVec3TransformCoord(&bulletVector, &bulletVector, &mat);
			D3DXVec3Normalize(&bulletVector, &bulletVector);
			OBJECTMANAGER->AddObject(OBJ_EFFECT,
				new Effect("./Resource/Effect/dust/dust_%d.png", 1, 5, pos, bulletVector * 20, 0.1f));
		}
		break;
	default:
		break;
	}
}

void Bullet::BulletEnemy(BULLET_FUNC_STATE state)
{
	switch (state)
	{
	case BULLET_FUNC_STATE::BULLET_INIT:
		animeMesh.push_back(MESHMANAGER->AddAnimeMesh("enemyBullet", L"./Resource/Enemy/enemy_attack/%d.obj", 0, 10));
		damage = 1;
		scale = { 0.03, 0.03, 0.03 };
		speed = 10;
		break;
	case BULLET_FUNC_STATE::BULLET_UPDATE:
		pos += moveVector * ELTIME * speed;
		RotateLerp(pos, moveVector, &matR);
		break;
	case BULLET_FUNC_STATE::BULLET_RENDER:
		MESHMANAGER->DrawAlphaMesh(animeMesh[0][int(frame * 12) % animeMesh[0].size()], pos, matR, scale);
		break;
	case BULLET_FUNC_STATE::BULLET_RELEASE:
		OBJECTMANAGER->AddObject(OBJ_EFFECT,
			new Effect("./Resource/Effect/attack_effect/%d.png", 1, 5, pos, { 0, 0, 0 }, 0.1f));
		break;
	default:
		break;
	}
}
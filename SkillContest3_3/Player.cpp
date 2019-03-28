#include "DXUT.h"
#include "Player.h"

#include "Background.h"
#include "CharacterAttack.h"
#include "Bullet.h"
#include "Particle.h"
#include "Item.h"
#include "UI.h"
#include "Enemy.h"
#include "PlayerMainUI.h"
#include "Effect.h"
#include "EndingScene.h"



Player::Player(Background *_background)
{
	background = _background;
}


Player::~Player()
{
	itemCount.clear();
}

void Player::Init()
{
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_Idle", L"./Resource/Player/player_idle/player_idle%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_move", L"./Resource/Player/player_move/player_move%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_Idle", L"./Resource/Player/player_idle/player_idle%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_Idle1", L"./Resource/Player/player_idle_1_upgrade/player_idle_1_upgrade%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_move1", L"./Resource/Player/player_move_1_upgrade/player_move_1_upgrade%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_Idle1", L"./Resource/Player/player_idle_1_upgrade/player_idle_1_upgrade%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_Idle2", L"./Resource/Player/player_idle_2_upgrade/player_idle_2_upgrade%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_move2", L"./Resource/Player/player_move_2_upgrade/player_move_2_upgrade%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_Idle2", L"./Resource/Player/player_idle_2_upgrade/player_idle_2_upgrade%d.obj", 0, ANIMEFRAME));

	distanceGround = 4.6f;
	pos = { 10, distanceGround, 0 };
	scale = { 0.035f, 0.035f, 0.035f };
	state = PLAYER_IDLE;
	moveVector = { 1, 0, 0 };
	isControl = false;
	radius = 3.0f;

	shield = MESHMANAGER->AddMesh("speedUpModel", L"./Resource/Effect/speed_up_model/speed_up.obj");

	//speedUpMesh = MESHMANAGER->AddMesh();

	playerFunc[PLAYER_IDLE] = &Player::PlayerIdle;
	playerFunc[PLAYER_MOVE] = &Player::PlayerMove;
	playerFunc[PLAYER_ATTACK] = &Player::PlayerAttack;

	vParticle.push_back(OBJECTMANAGER->AddObject(OBJ_EFFECT, new Particle("./Resource/Effect/dust/dust_%d.png", 1, 5, 0.1f, 0.6f)));
	speedUpAir = OBJECTMANAGER->AddObject(OBJ_UIEFFECT, new UI({640, 360}, "./Resource/Effect/speed_up/%d.png", 1, 4, true, 15));
	speedUpAir->isActive = false;
	nuclear = OBJECTMANAGER->AddObject(OBJ_UISCREEN, new UI({640, 360}, "./Resource/nuclear_effect/%d.png", 1, 31, false, 12));
	nuclear->isActive = false;
	mainUI = OBJECTMANAGER->AddObject(OBJ_KINDS::OBJ_UI, new PlayerMainUI(this, {640, 360}, "./Resource/UI/Ingame.png", 0, 0));
	mainUI->isActive = true;
	plane = MESHMANAGER->AddMesh("Plane", L"./Resource/Effect/Plane/bullet.obj");
	respawn = OBJECTMANAGER->AddObject(OBJ_UIEFFECT, new UI({640, 360}, "./Resource/respawn/%d.png", 1, 3, true, 1));
	respawn->isActive = false;
	speed = 0.3f;
	velocity = 0.0f;
	gravity = GRAVITY_SCALE;

	frontPoint = { 0, 0 };
	backPoint = { 0, 0 };
	isSpeedUp = false;
	speedUpFrame = 0.0f;
	lengthUpFrame = 0.0f;
	isJump = false;
	trakingFrame = 0.0f;
	jumpCount = 1;
	playerDieFrame = 0;
	isPlayerDie = false;
	isInvinc = false;
	upgrade = 0;

	hp = 1;
	totalHp = 3;

	for(int i = 0; i < 6; i++)
		itemCount.push_back(0);
}

void Player::Update()
{
	isControl = false;
	if (playerDieFrame < frame)
	{
		ItemUpdate();
		if (INPUTMANAGER->KeyDown(VK_SPACE) && jumpCount > 0)
		{
			PlayerJump();
			jumpCount--;
		}
		PlayerVelocity();
		(this->*playerFunc[state])();
		RotateLerp({ 0, 0, 0 }, moveVector, &matR);
	}
	if(!isInvinc)
		PlayerAttacked();
	if (INPUTMANAGER->KeyDown(VK_F1))
		isInvinc = true;
	if (INPUTMANAGER->KeyDown(VK_F2))
		upgrade++;
	if (upgrade > 2)
		upgrade = 2;

	frame += ELTIME;
}

void Player::Render()
{
	int count = int(frame * 15) % animeMesh[state + (3 * upgrade)].size();
	MESHMANAGER->DrawMesh(animeMesh[state + (3 * upgrade)][count], pos, matR, scale);
	if (isSpeedUp)
		MESHMANAGER->DrawAlphaMesh(shield, pos, matR, { 0.1, 0.1, 0.1 });
}

void Player::Release()
{
}

void Player::PlayerIdle()
{
	if (isControl)
		state = PLAYER_MOVE;
	PlayerAttackStart();
}

void Player::PlayerMove()
{
	pos += moveVector * speed;
	PlayerAttackStart();
	float number = d3d::GetRandomNumber(30, 100);
	vParticle[0]->isActive = true;
	vParticle[0]->ParticleInit({ pos.x, pos.y - distanceGround + 1.0f, pos.z }, -moveVector * number * 0.1f, number * 0.001f, false);
	if (!isControl)
	{
		state = PLAYER_IDLE;
		vParticle[0]->isActive = false;
	}
}

void Player::PlayerAttack()
{
	attack.AttackUpdate();
	if (!attack.isAttack)
		state = PLAYER_IDLE;
}

Vector3 Player::PlayerController(bool * isControl)
{
	Vector3 temp = { 0, 0, 0 };
	if (INPUTMANAGER->KeyPress(VK_LEFT))
	{
		temp += {-1, 0, 0};
		*isControl = true;
	}
	if (INPUTMANAGER->KeyPress(VK_RIGHT))
	{
		temp += {1, 0, 0};
		*isControl = true;
	}
	if (INPUTMANAGER->KeyPress(VK_UP))
	{
		temp += {0, 0, 1};
		*isControl = true;
	}
	if (INPUTMANAGER->KeyPress(VK_DOWN))
	{
		temp += {0, 0, -1};
		*isControl = true;
	}
	return temp;
}

void Player::PlayerVelocity()
{
	velocity -= gravity;
	Vector3 tempVector = PlayerController(&isControl);
	if (isControl)
	{
		moveVector = tempVector;
		PlayerPointCheck();
	}
	Vector2 groundCheck;
	groundCheck.x = pos.x;
	groundCheck.y = pos.y - distanceGround;
	Vector2 topCheck;
	topCheck.x = pos.x;
	topCheck.y = pos.z;
	if (CollisionGround(&groundCheck, background->minimap1) && GetPixelCollision(topCheck, background->minimap2))
	{
		pos.y = groundCheck.y + distanceGround;
		velocity = 0;
		if (isJump)
		{
			jumpCount++;
			isJump = false;
		}
	}
	pos.y += velocity;
}

void Player::PlayerPointCheck()
{
	frontPoint = { pos.x + moveVector.x * 3, pos.y + moveVector.y - distanceGround };
	backPoint = { pos.x - moveVector.x * 3, pos.y + moveVector.y - distanceGround };

	CollisionGround(&frontPoint, background->minimap1);
	CollisionGround(&backPoint, background->minimap1);

	Vector2 temp = frontPoint - backPoint;
	D3DXVec2Normalize(&temp, &temp);
	moveVector.x = temp.x;
	moveVector.y = temp.y;

}

bool Player::CollisionGround(Vector2 *p, Texture *tex)
{
	bool isCollision = false;
	if (GetPixelCollision(*p * 10, tex))
	{
		while(true)
		{
			if (GetPixelCollision(*p * 10, tex))
			{
				p->y += 0.1;
				isCollision = true;
			}
			else
			{
				p->y -= 0.1;
				break;
			}
		}
	}
	else
	{
		for (int i = 0; i < 30; i++)
		{
			if (GetPixelCollision(*p * 10, tex))
			{
				p->y += 0.1;
				//isCollision = true;
				break;
			}
			else
				p->y -= 0.1;
		}
	}
	return isCollision;
}

void Player::PlayerAttackStart()
{
	if (INPUTMANAGER->KeyDown('Q'))
	{
		state = PLAYER_ATTACK;
		if (trakingFrame < frame)
		{
			if (lengthUpFrame > frame)
				attack.Attack(OBJ_KINDS::OBJ_PBULLET, BULLET_STATE::BULLET_120MM, pos, moveVector, GRAVITY_SCALE);
			else
				attack.Attack(OBJ_KINDS::OBJ_PBULLET, BULLET_STATE::BULLET_120MM, pos, moveVector, GRAVITY_SCALE + 0.04f);
		}
		else
		{
			TrakingAttack();
		}
		attack.attackDelay = 0.5f;
		SOUNDMANAGER->DuplicatePlay("attack");
	}
}

void Player::PlayerJump()
{
	isJump = true;
	pos.y += 0.1f;
	velocity = 0.6f;
}

void Player::GetItem()
{
	auto iter = OBJECTMANAGER->GetObjectList(OBJ_KINDS::OBJ_ITEM);
	for (auto _iter = iter->begin(); _iter != iter->end(); _iter++)
	{
		if ((*_iter)->GetCircleCollision(pos, (*_iter)->pos, radius, (*_iter)->radius))
		{
			Item *item = dynamic_cast<Item*>(*_iter);
			itemCount[item->itemKind] += 1;
			item->isDestroy = true;
		}
	}
}

void Player::ItemUpdate()
{
	GetItem();
	if (INPUTMANAGER->KeyDown('1') && itemCount[ITEM_JUMP] > 0)
	{
		jumpCount++;
		itemCount[ITEM_JUMP]--;
	}
	if (INPUTMANAGER->KeyDown('2') && itemCount[ITEM_SPEEDUP] > 0)
	{
		speedUpFrame = frame + 2.0f;
		isSpeedUp = true;
		speed *= 1.5f;
		itemCount[ITEM_SPEEDUP]--;
		speedUpAir->isActive = true;
	}
	if (INPUTMANAGER->KeyDown('3') && itemCount[ITEM_LENGTHUP] > 0)
	{
		lengthUpFrame = frame + 10.0f;
		itemCount[ITEM_LENGTHUP]--;
	}
	if (INPUTMANAGER->KeyDown('5') && itemCount[ITEM_TRAKINGMISSILE] > 0)
	{
		trakingFrame = frame + 5.0f;
		itemCount[ITEM_TRAKINGMISSILE]--;
	}
	if (INPUTMANAGER->KeyDown('6') && itemCount[ITEM_NUCLEAR] > 0)
	{
		timeScale = 0.0f;
		nuclear->isActive = true;
		nuclear->animeEnd = false;
		itemCount[ITEM_NUCLEAR]--;
		SOUNDMANAGER->DuplicatePlay("yjhbs2");

		auto iter = OBJECTMANAGER->GetObjectList(OBJ_ENEMY);
		for (auto _iter = iter->begin(); _iter != iter->end(); _iter++)
		{
			Enemy *enemy = dynamic_cast<Enemy*>((*_iter));
			if (enemy->IsInScreen())
			{
				enemy->hp = 0;
			}
		}
	}
	if (nuclear->animeEnd)
	{
		nuclear->isActive = false;
		nuclear->frame = 0;
		timeScale = 1.0f;
	}
	if (speedUpFrame < frame)
	{
		speed = 0.3f;
		isSpeedUp = false;
		speedUpAir->isActive = false;
	}
}

void Player::TrakingAttack()
{
	auto iter = OBJECTMANAGER->GetObjectList(OBJ_ENEMY);
	vAttack.clear();
	int i = 0;
	for (auto _iter = iter->begin(); _iter != iter->end(); _iter++)
	{
		Enemy *enemy = (Enemy*)((*_iter));
		if (enemy->IsInScreen())
		{
			vAttack.push_back(CharacterAttack());
			vAttack[i].Attack(OBJ_PBULLET, BULLET_STATE::BULLET_TARGET, pos, moveVector, 0, enemy);
			i++;
		}
	}
}

void Player::PlayerAttacked()
{
	if (!isPlayerDie)
	{
		auto iter = OBJECTMANAGER->GetObjectList(OBJ_EBULLET);
		for (auto _iter = iter->begin(); _iter != iter->end(); _iter++)
		{
			if ((*_iter)->GetCircleCollision(pos, (*_iter)->pos, radius, (*_iter)->radius))
			{
				hp -= 1;
			}
		}
	}
	if (pos.y < -10)
	{
		hp = 0;
	}
	if (hp < 1 && isPlayerDie == false)
	{
		totalHp -= 1;
		playerDieFrame = frame + 2.9f;
		respawn->isActive = true;
		isPlayerDie = true;
		OBJECTMANAGER->AddObject(OBJ_EFFECT,
			new Effect("./Resource/Effect/bomb/bomb_%d.png", 1, 8, pos, { 0, 0, 0 }, 0.3f));
	}
	if (isPlayerDie == true && playerDieFrame < frame)
	{
		respawn->isActive = false;
		isPlayerDie = false;
		hp = 1;
		pos = DATAMANAGER->savePoint;
	}
	if (totalHp < 1)
	{
		SCENEMANAGER->AddScene(new EndingScene(false));
	}
}

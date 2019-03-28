#include "DXUT.h"
#include "PlayerArm.h"

#include "Player.h"
#include "CharacterAttack.h"
#include "Bullet.h"
#include "Item.h"


PlayerArm::PlayerArm(Player * _player)
{
	player = _player;
	pos = player->pos;
	scale = player->scale;
}

PlayerArm::~PlayerArm()
{
}

void PlayerArm::Init()
{
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_arm_Idle", L"./Resource/Player/player_arm/player_arm%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_arm_Idle", L"./Resource/Player/player_arm/player_arm%d.obj", 0, ANIMEFRAME));
	animeMesh.push_back(MESHMANAGER->AddAnimeMesh("Player_arm_Idle", L"./Resource/Player/player_arm_attack/player_arm_attack%d.obj", 0, ANIMEFRAME));

	state = PLAYER_IDLE;

	playerArmFunc[PLAYER_IDLE] = &PlayerArm::PlayerArmIdle;
	playerArmFunc[PLAYER_ATTACK] = &PlayerArm::PlayerArmAttack;

	armRotate = 0;
	attack[0].attackDelay = 0.3f;
	attack[1].attackDelay = 0.3f;
	attack[2].attackDelay = 0.3f;

	thirdFrame = 0.0f;
}

void PlayerArm::Update()
{
	pos = player->pos;
	moveVector = player->moveVector;
	moveVector.y += armRotate;
	RotateLerp({0, 0, 0}, moveVector, &matR);

	if (player->isPlayerDie) return;
	(this->*playerArmFunc[state])();
	if (INPUTMANAGER->KeyDown('A'))
	{
		armRotate += 0.5f;
		if (armRotate >= 1)
			armRotate += 3;
		if (armRotate > 4)
			armRotate = 0;
	}
	if (INPUTMANAGER->KeyDown('4') && player->itemCount[ITEM_THIRD])
	{
		thirdFrame = frame + 10.0f;
		player->itemCount[ITEM_THIRD]--;
	}

	frame += ELTIME;
}

void PlayerArm::Render()
{
	int count = int(frame * 15) % animeMesh[state].size();
	MESHMANAGER->DrawMesh(animeMesh[state][count], pos, matR, scale);
}

void PlayerArm::Release()
{
}

void PlayerArm::PlayerArmIdle()
{
	if (INPUTMANAGER->KeyDown('W'))
	{
		state = PLAYER_STATE::PLAYER_ATTACK;
	}
}

void PlayerArm::PlayerArmAttack()
{
	if (INPUTMANAGER->KeyDown('W'))
	{
		state = PLAYER_STATE::PLAYER_IDLE;
	}

	Vector3 bulletVector = { 0, 0, 1};
	D3DXVec3TransformCoord(&bulletVector, &bulletVector, &matR);
	D3DXVec3Normalize(&bulletVector, &bulletVector);

	if (thirdFrame < frame)
	{
		for (int i = 0; i < 2; i++)
		{
			Matrix tempMat;
			Vector3 startVector = { 0, 0, 1 };
			D3DXMatrixRotationY(&tempMat, D3DXToRadian(-30 + 60 * i));
			tempMat *= matR;
			D3DXVec3TransformNormal(&startVector, &startVector, &tempMat);
			if (attack[i].isAttack == false)
			{
				SOUNDMANAGER->DuplicatePlay("fire");
				SOUNDMANAGER->DuplicatePlay("yjhbs");
			}
			attack[i].Attack(OBJ_PBULLET, BULLET_STATE::BULLET_88MM, pos + startVector * 3.0f, bulletVector, 0);
			attack[i].AttackUpdate();
		}
	}
	else
	{
		for (int i = 0; i < 3; i++)
		{
			Matrix tempMat;
			Vector3 startVector = { 0, 0, 1 };
			D3DXMatrixRotationY(&tempMat, D3DXToRadian(-30 + 30 * i));
			tempMat *= matR;
			D3DXVec3TransformNormal(&startVector, &startVector, &tempMat);
			attack[i].Attack(OBJ_PBULLET, BULLET_STATE::BULLET_88MM, pos + startVector * 3.0f, startVector, 0);
			attack[i].AttackUpdate();
		}
	}
}

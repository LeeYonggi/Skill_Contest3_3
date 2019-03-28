#pragma once
#include "Object.h"
#include "CharacterAttack.h"


enum PLAYER_STATE
{
	PLAYER_IDLE,
	PLAYER_MOVE,
	PLAYER_ATTACK
};
class UI;
class Background;
class CharacterAttack;
class Player :
	public Object
{
	using Playerfunc = void(Player::*)(void);
public:
	Player(Background *_background);
	~Player();

private:
	PLAYER_STATE state;
	Playerfunc playerFunc[PLAYER_ATTACK + 1];
	Vector2 frontPoint;
	Vector2 backPoint;
	CharacterAttack attack;
	Background *background;
	CMeshLoader *speedUpMesh;
	float speed;
	float distanceGround;
	bool isControl;
	bool isJump;
	int jumpCount;
	float velocity;
	float gravity;
	bool isSpeedUp;
	float speedUpFrame;
	float lengthUpFrame;
	float trakingFrame;
	vector<CharacterAttack> vAttack;
	CMeshLoader *plane;
	CMeshLoader *shield;
	UI *speedUpAir;
	UI *nuclear;
	UI *mainUI;
	UI *respawn;
	float playerDieFrame;
	bool isInvinc;

public:
	bool isPlayerDie;
	vector<int> itemCount;
	int hp;
	int totalHp;
	int upgrade;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;

public:
	void PlayerIdle();
	void PlayerMove();
	void PlayerAttack();

public:
	Vector3 PlayerController(bool *isControl);
	void PlayerVelocity();
	void PlayerPointCheck();
	bool CollisionGround(Vector2 *p, Texture *tex);
	void PlayerAttackStart();
	void PlayerJump();
	void GetItem();
	void ItemUpdate();
	void TrakingAttack();
	void PlayerAttacked();
};


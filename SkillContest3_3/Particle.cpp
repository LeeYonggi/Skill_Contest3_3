#include "DXUT.h"
#include "Particle.h"

#include "Effect.h"

Particle::Particle(string route, int low, int high, float startTime, float endTime)
{
	this->route = route;
	this->low = low;
	this->high = high;
	this->startTime = startTime;
	this->endTime = endTime;
	isActive = false;
}

Particle::~Particle()
{
}

void Particle::Init()
{
}

void Particle::Update()
{
	if (frame > startTime)
	{
		OBJECTMANAGER->AddObject(OBJ_EFFECT, new Effect(route, low, high, pos, moveVector, scale.x, isLoop, endTime));
		frame = 0;
	}
	frame += ELTIME;
}

void Particle::Render()
{
}

void Particle::Release()
{
}

void Particle::ParticleInit(Vector3 _pos, Vector3 _moveVector, float _scale, bool _isLoop)
{
	pos = _pos;
	moveVector = _moveVector;
	scale = { _scale, _scale, _scale };
	isLoop = _isLoop;
}

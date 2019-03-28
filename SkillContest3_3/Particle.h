#pragma once
#include "Object.h"
class Particle :
	public Object
{
public:
	Particle(string route, int low, int high, float startTime, float endTime);
	virtual ~Particle();

private:
	string route;
	int low;
	int high;
	float startTime;
	float endTime;
	bool isLoop;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;

public:
	void ParticleInit(Vector3 _pos, Vector3 _moveVector, float _scale, bool _isLoop);
};


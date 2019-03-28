#pragma once
#include "Object.h"
class Effect :
	public Object
{
public:
	Effect(string route, int low, int high, Vector3 _pos, Vector3 _moveVector, float _scale, bool _isLoop, float _dieTime);
	Effect(string route, int low, int high, Vector3 _pos, Vector3 _moveVector, float _scale);
	virtual ~Effect();

private:
	bool isLoop;
	float dieTime;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;

};


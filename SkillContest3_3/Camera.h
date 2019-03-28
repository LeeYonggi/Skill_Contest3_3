#pragma once
#include "Object.h"
class Camera :
	public Object
{
public:
	Camera();
	virtual ~Camera();

public:
	Vector3 eye;
	Vector3 at;
	Vector3 up;

public:
	bool isCameraShake = false;
	float shakeDelay;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;

	void CameraShake(float time);
};


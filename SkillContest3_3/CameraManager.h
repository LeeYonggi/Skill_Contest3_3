#pragma once
#include "Singleton.h"

class Camera;
class CameraManager :
	public Singleton<CameraManager>
{
public:
	CameraManager();
	virtual ~CameraManager();

public:
	Camera *mainCamera;

public:
	void SetCamera(Camera *camera);
	void CameraShake(float time);
};

#define CAMERAMANAGER CameraManager::GetInstance()
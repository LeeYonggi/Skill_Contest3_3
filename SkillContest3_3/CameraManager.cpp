#include "DXUT.h"
#include "CameraManager.h"

#include "Camera.h"

CameraManager::CameraManager()
{
	mainCamera = nullptr;
}


CameraManager::~CameraManager()
{
}

void CameraManager::SetCamera(Camera * camera)
{
	mainCamera = camera;
}

void CameraManager::CameraShake(float time)
{
	mainCamera->CameraShake(time);
}

#include "DXUT.h"
#include "Camera.h"


Camera::Camera()
{
	eye = { 0, 0, -10};
	at = { 0, 0, 0 };
	up = { 0, 1, 0 };
}


Camera::~Camera()
{
}

void Camera::Init()
{
	isCameraShake = false;
	shakeDelay = 0;
}

void Camera::Update()
{
	if (isCameraShake && frame < shakeDelay)
	{
		eye.x += d3d::GetRandomNumber(-3, 3) * ELTIME * 40;
		eye.y += d3d::GetRandomNumber(-3, 3) * ELTIME * 40;
	}
	frame += ELTIME;
}

void Camera::Render()
{
	Matrix mat, proj;
	D3DXMatrixLookAtLH(&mat, &eye, &at, &up);

	DEVICE->SetTransform(D3DTS_VIEW, &mat);
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(60), (float)SCREEN_X / (float)SCREEN_Y, 1.0f, 1000000.0f);
	DEVICE->SetTransform(D3DTS_PROJECTION, &proj);
}

void Camera::Release()
{
}

void Camera::CameraShake(float time)
{
	isCameraShake = true;
	shakeDelay = frame + time;
}

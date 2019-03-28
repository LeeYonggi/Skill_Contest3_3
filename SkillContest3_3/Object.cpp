#include "DXUT.h"
#include "Object.h"

#include "Particle.h"

Object::Object()
{
	pos = { 0, 0, 0 };
	scale = { 1, 1, 1 };
	D3DXMatrixIdentity(&matR);
	D3DXQuaternionIdentity(&quaternion);
	texture = nullptr;
	mesh = nullptr;
	isDestroy = false;
	isActive = true;
	frame = 0;
	moveVector = {1, 0, 0};
	radius = 1.0f;
}


Object::~Object()
{
	for (auto iter : animeTexture)
		iter.clear();
	for (auto iter : animeMesh)
		iter.clear();
	for (auto iter : vParticle)
	{
		if (iter)
			iter->isDestroy = true;
	}
	vParticle.clear();
	animeTexture.clear();
	animeMesh.clear();
}

void Object::RotateLerp(Vector3 eye, Vector3 at, Matrix * mat)
{
	Matrix nowMat;
	D3DXMatrixLookAtLH(&nowMat, &eye, &at, &Vector3(0, 1, 0));
	D3DXMatrixTranspose(&nowMat, &nowMat);

	D3DXQUATERNION nowQuater;
	D3DXQuaternionRotationMatrix(&nowQuater, &nowMat);
	D3DXQuaternionSlerp(&quaternion, &quaternion, &nowQuater, 0.1f);
	D3DXMatrixRotationQuaternion(&matR, &quaternion);
}

bool Object::GetPixelCollision(Vector2 p1, Texture *tex)
{
	bool isCollision = false;
	D3DLOCKED_RECT lock = { 0 };
	tex->tex->LockRect(0, &lock, nullptr, D3DLOCK_DISCARD);

	int x = p1.x;
	int y = (tex->info.Height * 0.5) - p1.y;

	DWORD *color = (DWORD*)lock.pBits;

	if (x < 0 || x > tex->info.Width ||
		y < 0 || y > tex->info.Height)
	{
		tex->tex->UnlockRect(0);
		return false;
	}

	if (color[(y * tex->info.Width) + x] == D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f))
		isCollision = true;

	tex->tex->UnlockRect(0);
	return isCollision;
}

Matrix Object::GetBillboard(Vector3 eye, Vector3 at)
{
	Matrix mat, tempMat;
	Vector3 temp = at - eye;
	D3DXVec3Normalize(&temp, &temp);
	D3DXMatrixLookAtLH(&mat, &Vector3(0, 0, 0), &temp, &Vector3(0, 1, 0));
	D3DXMatrixRotationX(&tempMat, D3DXToRadian(-90));
	return mat * tempMat;
}

Vector3 Object::GetTargetVec3(Vector3 p1, Vector3 p2)
{
	Vector3 temp = p2 - p1;
	D3DXVec3Normalize(&temp, &temp);
	return temp;
}

bool Object::GetCircleCollision(Vector3 p1, Vector3 p2, float r1, float r2)
{
	Vector3 temp = p2 - p1;
	float length = temp.x * temp.x + temp.y * temp.y;
	float d = length + temp.z * temp.z;

	return (sqrt(d) < r1 + r2);
}

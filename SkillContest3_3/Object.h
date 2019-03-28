#pragma once

#define GRAVITY_SCALE 0.02
class Particle;
class Object
{
public:
	Object();
	virtual ~Object();

public:
	Vector3 pos;
	Vector3 scale;
	Matrix matR;
	Texture *texture;
	CMeshLoader *mesh;
	vector<vector<Texture*>> animeTexture;
	vector<vector<CMeshLoader*>> animeMesh;
	vector<Particle*> vParticle;
	Vector3 moveVector;
	bool isDestroy;
	bool isActive;
	float radius;
	float frame;

protected:
	D3DXQUATERNION quaternion;

public:
	virtual void Init()		PURE;
	virtual void Update()	PURE;
	virtual void Render()	PURE;
	virtual void Release()	PURE;

public:
	void RotateLerp(Vector3 eye, Vector3 at, Matrix *mat);
	bool GetPixelCollision(Vector2 p1, Texture *tex);
	Matrix GetBillboard(Vector3 eye, Vector3 at);
	Vector3 GetTargetVec3(Vector3 p1, Vector3 p2);
	bool GetCircleCollision(Vector3 p1, Vector3 p2, float r1, float r2);
};


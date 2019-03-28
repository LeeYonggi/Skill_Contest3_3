#pragma once

#pragma comment(lib, "dsound.lib")
//define
#define ELTIME DXUTGetElapsedTime() * timeScale
#define SCREEN_X 1280
#define SCREEN_Y 720
#define DEVICE DXUTGetD3D9Device()

extern float timeScale;
typedef D3DXVECTOR3	Vector3;
typedef D3DXVECTOR2	Vector2;
typedef D3DXMATRIX	Matrix;


//custom

#include "Utility.h"

#include "Singleton.h"
#include "SoundManager.h"

#include "ImageManager.h"

#include "MeshLoader.h"
#include "MeshManager.h"

#include "Object.h"
#include "ObjectManager.h"

#include "Scene.h"
#include "SceneManager.h"

#include "InputManager.h"

#include "Camera.h"
#include "CameraManager.h"

#include "DataManager.h"

#include "LoadScene.h"
#include "IngameScene.h"
#include "MainProc.h"
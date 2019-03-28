#include "DXUT.h"
#include "MainProc.h"

#include "IngameScene.h"
#include "LoadScene.h"

MainProc::MainProc()
{
}


MainProc::~MainProc()
{
}

void MainProc::Init()
{
	DATAMANAGER->Init();
	SCENEMANAGER->AddScene(new LoadScene());
}

void MainProc::Update()
{
	OBJECTMANAGER->Update();
	SCENEMANAGER->Update();
	DATAMANAGER->Update();
	INPUTMANAGER->Update();
}

void MainProc::Render()
{
	OBJECTMANAGER->Render();
	SCENEMANAGER->Render();
	DATAMANAGER->Render();
}

void MainProc::Release()
{
	MeshManager::ReleaseInstance();
	ImageManager::ReleaseInstance();
	ObjectManager::ReleaseInstance();
	SceneManager::ReleaseInstance();
	InputManager::ReleaseInstance();
	DataManager::ReleaseInstance();
}

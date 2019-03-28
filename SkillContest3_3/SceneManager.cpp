#include "DXUT.h"
#include "Singleton.h"


SceneManager::SceneManager()
{
	nowScene = nullptr;
	nextScene = nullptr;
	isSceneChange = false;
}


SceneManager::~SceneManager()
{
	Release();
}

void SceneManager::AddScene(Scene * scene)
{
	nextScene = scene;
	isSceneChange = true;
}

void SceneManager::Update()
{
	if (isSceneChange)
	{
		if (nowScene)
		{
			nowScene->Release();
			SAFE_DELETE(nowScene);
			nowScene = nullptr;
			OBJECTMANAGER->Release();
			OBJECTMANAGER->Init();
			DATAMANAGER->StartSceneChange();
		}
		nowScene = nextScene;
		nextScene = nullptr;
		nowScene->Init();
		isSceneChange = false;
	}
	if (nowScene)
		nowScene->Update();
}

void SceneManager::Render()
{
	if (nowScene)
		nowScene->Render();
}

void SceneManager::Release()
{
	SAFE_RELEASE(nowScene);
	SAFE_RELEASE(nextScene);
	SAFE_DELETE(nowScene);
	SAFE_DELETE(nextScene);
}

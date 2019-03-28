#pragma once
#include "Singleton.h"

enum OBJ_KINDS
{
	OBJ_CREATER,
	OBJ_CAMERA,
	OBJ_BACKGROUND,
	OBJ_ENEMY,
	OBJ_PLAYER,
	OBJ_ITEM,
	OBJ_SAVEPOINT,
	OBJ_PBULLET,
	OBJ_EBULLET,
	OBJ_EFFECT,
	OBJ_UIEFFECT,
	OBJ_UI,
	OBJ_UISCREEN,
	OBJ_LAST
};
class ObjectManager :
	public Singleton<ObjectManager>
{
public:
	ObjectManager();
	virtual ~ObjectManager();

private:
	map<OBJ_KINDS, list<Object*>*> m_Object;

public:
	void Init();
	void Update();
	void Render();
	void Release();

public:
	template<class T>
	T *AddObject(OBJ_KINDS kind, T *obj)
	{
		auto iter = m_Object.find(kind);
		if (iter == m_Object.end()) return nullptr;

		obj->Init();

		iter->second->push_back(obj);

		return obj;
	}

	list<Object*>* GetObjectList(OBJ_KINDS kind);
};

#define OBJECTMANAGER ObjectManager::GetInstance()
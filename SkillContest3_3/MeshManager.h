#pragma once
#include "Singleton.h"
class MeshManager :
	public Singleton<MeshManager>
{
public:
	MeshManager();
	virtual ~MeshManager();

private:
	map<string, CMeshLoader*> m_MeshLoader;
	map<wstring, vector<Material*>*> m_Material;

public:
	CMeshLoader* AddMesh(string name, wstring file);
	vector<CMeshLoader*> AddAnimeMesh(string name, wstring file, int low, int high);
	vector<Material*> *AddMaterialList(wstring file, vector<Material*> *materialList);
	void DrawMesh(CMeshLoader *mesh, Vector3 pos, Matrix matR, Vector3 scale);
	void DrawEffect(CMeshLoader *mesh, Texture *texture, Vector3 pos, Matrix matR, Vector3 scale);
	void DrawAlphaMesh(CMeshLoader *mesh, Vector3 pos, Matrix matR, Vector3 scale);
	void Release();
};

#define MESHMANAGER MeshManager::GetInstance()
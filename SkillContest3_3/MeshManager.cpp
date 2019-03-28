#include "DXUT.h"
#include "MeshManager.h"


MeshManager::MeshManager()
{
}


MeshManager::~MeshManager()
{
	Release();
}

CMeshLoader * MeshManager::AddMesh(string name, wstring file)
{
	auto iter = m_MeshLoader.find(name);
	if (iter != m_MeshLoader.end()) return iter->second;

	CMeshLoader *temp = new CMeshLoader;

	temp->Create(DEVICE, file.c_str());

	m_MeshLoader.insert(make_pair(name, temp));

	return temp;
}

vector<CMeshLoader*> MeshManager::AddAnimeMesh(string name, wstring file, int low, int high)
{
	vector<CMeshLoader*> temp;
	for (int i = low; i <= high; ++i)
	{
		WCHAR wctr[128];
		wsprintf(wctr, file.c_str(), i);
		temp.push_back(AddMesh(name + to_string(i), wctr));
	}
	return temp;
}

vector<Material*>* MeshManager::AddMaterialList(wstring file, vector<Material*> *materialList)
{
	auto iter = m_Material.find(file);
	if (iter != m_Material.end()) return iter->second;

	m_Material.insert(make_pair(file, materialList));

	return nullptr;
}

void MeshManager::DrawMesh(CMeshLoader * mesh, Vector3 pos, Matrix matR, Vector3 scale)
{
	Matrix matW, matS, matT;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

	matW = matS * matR * matT;

	DEVICE->SetTransform(D3DTS_WORLD, &matW);

	DEVICE->SetRenderState(D3DRS_LIGHTING, true);

	Vector3 dir = { 1.0f, -1.0f, 1.0f};
	D3DLIGHT9 light = d3d::GetDirectionLight(&dir, &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	DEVICE->LightEnable(0, true);
	DEVICE->SetLight(0, &light);

	DEVICE->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	DEVICE->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	DEVICE->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	DEVICE->SetRenderState(D3DRS_SPECULARENABLE, true);

	for (int i = 0; i < mesh->GetNumMaterials(); ++i)
	{
		D3DMATERIAL9 mtl = mesh->GetMaterial(i)->mat;
		mtl.Ambient.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Diffuse.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Specular.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Emissive = D3DXCOLOR(mesh->GetMaterial(i)->mat.Diffuse) * 0.2f;
		mtl.Emissive.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Power = 1.0f;

		DEVICE->SetTexture(0, mesh->GetMaterial(i)->pTexture);
		DEVICE->SetMaterial(&mtl);
		mesh->GetMesh()->DrawSubset(i);
	}
}

void MeshManager::DrawEffect(CMeshLoader * mesh, Texture * texture, Vector3 pos, Matrix matR, Vector3 scale)
{

	Matrix matW, matS, matT;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

	matW = matS * matR * matT;

	DEVICE->SetTransform(D3DTS_WORLD, &matW);

	DEVICE->SetRenderState(D3DRS_LIGHTING, true);

	Vector3 dir = { 1.0f, -1.0f, 1.0f };
	D3DLIGHT9 light = d3d::GetDirectionLight(&dir, &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	DEVICE->LightEnable(0, true);
	DEVICE->SetLight(0, &light);

	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	for (int i = 0; i < mesh->GetNumMaterials(); ++i)
	{
		D3DMATERIAL9 mtl = mesh->GetMaterial(i)->mat;
		mtl.Ambient.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Diffuse.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Specular.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Emissive = D3DXCOLOR(mesh->GetMaterial(i)->mat.Diffuse) * 0.2f;
		mtl.Emissive.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Power = 1.0f;

		DEVICE->SetTexture(0, texture->tex);
		DEVICE->SetMaterial(&mtl);
		mesh->GetMesh()->DrawSubset(i);
	}

	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BOTHINVSRCALPHA);
}

void MeshManager::DrawAlphaMesh(CMeshLoader * mesh, Vector3 pos, Matrix matR, Vector3 scale)
{
	Matrix matW, matS, matT;
	D3DXMatrixScaling(&matS, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&matT, pos.x, pos.y, pos.z);

	matW = matS * matR * matT;

	DEVICE->SetTransform(D3DTS_WORLD, &matW);

	DEVICE->SetRenderState(D3DRS_LIGHTING, true);

	Vector3 dir = { 1.0f, -1.0f, 1.0f };
	D3DLIGHT9 light = d3d::GetDirectionLight(&dir, &D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	DEVICE->LightEnable(0, true);
	DEVICE->SetLight(0, &light);

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	for (int i = 0; i < mesh->GetNumMaterials(); ++i)
	{
		D3DMATERIAL9 mtl = mesh->GetMaterial(i)->mat;
		mtl.Ambient.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Diffuse.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Specular.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Emissive = D3DXCOLOR(mesh->GetMaterial(i)->mat.Diffuse) * 1.0f;
		mtl.Emissive.a = mesh->GetMaterial(i)->fAlpha;
		mtl.Power = 1.0f;

		DEVICE->SetTexture(0, mesh->GetMaterial(i)->pTexture);
		DEVICE->SetMaterial(&mtl);
		mesh->GetMesh()->DrawSubset(i);
	}

	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BOTHINVSRCALPHA);
}

void MeshManager::Release()
{
	for (auto iter : m_Material)
	{
		for (auto _iter : *iter.second)
		{
			SAFE_DELETE(_iter);
		}
		iter.second->clear();
	}
	m_Material.clear();

	for (auto iter : m_MeshLoader)
	{
		SAFE_DELETE(iter.second);
	}
	m_MeshLoader.clear();
}

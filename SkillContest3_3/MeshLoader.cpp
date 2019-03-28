//--------------------------------------------------------------------------------------
// File: MeshLoader.cpp
//
// Wrapper class for ID3DXMesh interface. Handles loading mesh data from an .obj file
// and resource management for material textures.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#include "SDKmisc.h"
#pragma warning(disable: 4995)
#include "MeshLoader.h"
#include <fstream>
using namespace std;
#pragma warning(default: 4995)


// Vertex declaration
D3DVERTEXELEMENT9 VERTEX_DECL[] =
{
	{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0},
	{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0},
	{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};


//--------------------------------------------------------------------------------------
CMeshLoader::CMeshLoader()
{
	m_pd3dDevice = NULL;
	m_pMesh = NULL;

	ZeroMemory(m_strMediaDir, sizeof(m_strMediaDir));
	isCopyMat = false;
}


//--------------------------------------------------------------------------------------
CMeshLoader::~CMeshLoader()
{
	Destroy();
}


//--------------------------------------------------------------------------------------
void CMeshLoader::Destroy()
{
	m_Vertices.RemoveAll();
	m_Indices.RemoveAll();
	m_Attributes.RemoveAll();

	SAFE_RELEASE(m_pMesh);
	m_pd3dDevice = NULL;
}


//--------------------------------------------------------------------------------------
HRESULT CMeshLoader::Create(IDirect3DDevice9* pd3dDevice, const WCHAR* strFilename)
{
	HRESULT hr;
	WCHAR str[MAX_PATH] = { 0 };

	// Start clean
	Destroy();

	// Store the device pointer
	m_pd3dDevice = pd3dDevice;

	// Load the vertex buffer, index buffer, and subset information from a file. In this case, 
	// an .obj file was chosen for simplicity, but it's meant to illustrate that ID3DXMesh objects
	// can be filled from any mesh file format once the necessary data is extracted from file.
	V_RETURN(LoadGeometryFromOBJ(strFilename));

	// Set the current directory based on where the mesh was found
	WCHAR wstrOldDir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, wstrOldDir);
	SetCurrentDirectory(m_strMediaDir);

	// Load material textures
	if (!isCopyMat)
	{
		for (int iMaterial = 0; iMaterial < m_Materials->size(); iMaterial++)
		{
			Material* pMaterial = (*m_Materials)[iMaterial];
			if (pMaterial->strTexture[0])
			{
				// Avoid loading the same texture twice
				bool bFound = false;
				for (int x = 0; x < iMaterial; x++)
				{
					Material* pCur = (*m_Materials)[x];
					if (0 == wcscmp(pCur->strTexture.c_str(), pMaterial->strTexture.c_str()))
					{
						bFound = true;
						pMaterial->pTexture = pCur->pTexture;
						break;
					}
				}

				// Not found, load the texture
				if (!bFound)
				{
					wstring wstr = strFilename;
					int size = wstr.rfind(L"/");
					wstr = wstr.substr(0, size + 1);

					pMaterial->strTexture = wstr + pMaterial->strTexture;

					string name;
					name.assign(pMaterial->strTexture.begin(), pMaterial->strTexture.end());
					pMaterial->pTexture = IMAGEMANAGER->AddTexture(name, name)->tex;
					V_RETURN(DXUTFindDXSDKMediaFileCch(str, MAX_PATH, pMaterial->strTexture.c_str()));
				}
			}
		}
	}

	// Restore the original current directory
	SetCurrentDirectory(wstrOldDir);

	// Create the encapsulated mesh
	ID3DXMesh* pMesh = NULL;
	V_RETURN(D3DXCreateMesh(m_Indices.GetSize() / 3, m_Vertices.GetSize(),
		D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_DECL,
		pd3dDevice, &pMesh));

	// Copy the vertex data
	VERTEX* pVertex;
	V_RETURN(pMesh->LockVertexBuffer(0, (void**)&pVertex));
	memcpy(pVertex, m_Vertices.GetData(), m_Vertices.GetSize() * sizeof(VERTEX));
	pMesh->UnlockVertexBuffer();
	m_Vertices.RemoveAll();

	// Copy the index data
	DWORD* pIndex;
	V_RETURN(pMesh->LockIndexBuffer(0, (void**)&pIndex));
	memcpy(pIndex, m_Indices.GetData(), m_Indices.GetSize() * sizeof(DWORD));
	pMesh->UnlockIndexBuffer();
	m_Indices.RemoveAll();

	// Copy the attribute data
	DWORD* pSubset;
	V_RETURN(pMesh->LockAttributeBuffer(0, &pSubset));
	memcpy(pSubset, m_Attributes.GetData(), m_Attributes.GetSize() * sizeof(DWORD));
	pMesh->UnlockAttributeBuffer();
	m_Attributes.RemoveAll();

	// Reorder the vertices according to subset and optimize the mesh for this graphics 
	// card's vertex cache. When rendering the mesh's triangle list the vertices will 
	// cache hit more often so it won't have to re-execute the vertex shader.
	DWORD* aAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
	if (aAdjacency == NULL)
		return E_OUTOFMEMORY;

	V(pMesh->GenerateAdjacency(1e-6f, aAdjacency));
	V(pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL));

	SAFE_DELETE_ARRAY(aAdjacency);
	m_pMesh = pMesh;

	return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CMeshLoader::LoadGeometryFromOBJ(const WCHAR* strFileName)
{
	wstring strMaterialFilename;
	WCHAR wstr[MAX_PATH];
	char str[MAX_PATH];
	HRESULT hr;

	m_Materials = new vector<Material*>;

	// Find the file
	V_RETURN(DXUTFindDXSDKMediaFileCch(wstr, MAX_PATH, strFileName));
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, MAX_PATH, NULL, NULL);

	// Store the directory where the mesh was found
	wcscpy_s(m_strMediaDir, MAX_PATH - 1, wstr);
	WCHAR* pch = wcsrchr(m_strMediaDir, L'\\');
	if (pch)
		*pch = NULL;

	// Create temporary storage for the input data. Once the data has been loaded into
	// a reasonable format we can create a D3DXMesh object and load it with the mesh data.
	CGrowableArray <D3DXVECTOR3> Positions;
	CGrowableArray <D3DXVECTOR2> TexCoords;
	CGrowableArray <D3DXVECTOR3> Normals;

	// The first subset uses the default material
	Material* pMaterial = new Material();
	if (pMaterial == NULL)
		return E_OUTOFMEMORY;

	InitMaterial(pMaterial);
	wcscpy_s(pMaterial->strName, MAX_PATH - 1, L"default");
	m_Materials->push_back(pMaterial);

	DWORD dwCurSubset = 0;

	// File input
	WCHAR strCommand[256] = { 0 };
	wifstream InFile(str);
	if (!InFile)
		return DXTRACE_ERR(L"wifstream::open", E_FAIL);

	for (; ; )
	{
		InFile >> strCommand;
		if (!InFile)
			break;

		if (0 == wcscmp(strCommand, L"#"))
		{
			// Comment
		}
		else if (0 == wcscmp(strCommand, L"v"))
		{
			// Vertex Position
			float x, y, z;
			InFile >> x >> y >> z;
			Positions.Add(D3DXVECTOR3(x, y, z));
		}
		else if (0 == wcscmp(strCommand, L"vt"))
		{
			// Vertex TexCoord
			float u, v;
			InFile >> u >> v;
			TexCoords.Add(D3DXVECTOR2(u, 1.0f - v));
		}
		else if (0 == wcscmp(strCommand, L"vn"))
		{
			// Vertex Normal
			float x, y, z;
			InFile >> x >> y >> z;
			Normals.Add(D3DXVECTOR3(x, y, z));
		}
		else if (0 == wcscmp(strCommand, L"f"))
		{
			// Face
			UINT iPosition, iTexCoord, iNormal;
			VERTEX vertex;

			for (UINT iFace = 0; iFace < 3; iFace++)
			{
				ZeroMemory(&vertex, sizeof(VERTEX));

				// OBJ format uses 1-based arrays
				InFile >> iPosition;
				vertex.position = Positions[iPosition - 1];

				if ('/' == InFile.peek())
				{
					InFile.ignore();

					if ('/' != InFile.peek())
					{
						// Optional texture coordinate
						InFile >> iTexCoord;
						vertex.texcoord = TexCoords[iTexCoord - 1];
					}

					if ('/' == InFile.peek())
					{
						InFile.ignore();

						// Optional vertex normal
						InFile >> iNormal;
						vertex.normal = Normals[iNormal - 1];
					}
				}

				// If a duplicate vertex doesn't exist, add this vertex to the Vertices
				// list. Store the index in the Indices array. The Vertices and Indices
				// lists will eventually become the Vertex Buffer and Index Buffer for
				// the mesh.
				DWORD index = AddVertex(iPosition, &vertex);
				if (index == (DWORD)-1)
					return E_OUTOFMEMORY;

				m_Indices.Add(index);
			}
			m_Attributes.Add(dwCurSubset);
		}
		else if (0 == wcscmp(strCommand, L"mtllib"))
		{
			// Material library
			InFile >> strMaterialFilename;

			wstring str = strFileName;
			int size = str.rfind(L"/");
			str = str.substr(0, size + 1);
			strMaterialFilename = str + strMaterialFilename;
		}
		else if (0 == wcscmp(strCommand, L"usemtl"))
		{
			// Material
			WCHAR strName[MAX_PATH] = { 0 };
			InFile >> strName;

			bool bFound = false;
			for (int iMaterial = 0; iMaterial < m_Materials->size(); iMaterial++)
			{
				Material* pCurMaterial = (*m_Materials)[iMaterial];
				if (0 == wcscmp(pCurMaterial->strName, strName))
				{
					bFound = true;
					dwCurSubset = iMaterial;
					break;
				}
			}

			if (!bFound)
			{
				pMaterial = new Material();
				if (pMaterial == NULL)
					return E_OUTOFMEMORY;

				dwCurSubset = m_Materials->size();

				InitMaterial(pMaterial);
				wcscpy_s(pMaterial->strName, MAX_PATH - 1, strName);

				m_Materials->push_back(pMaterial);
			}
		}
		else
		{
			// Unimplemented or unrecognized command
		}

		InFile.ignore(1000, '\n');
	}

	// Cleanup
	InFile.close();
	DeleteCache();

	// If an associated material file was found, read that in as well.
	if (strMaterialFilename[0])
	{
		V_RETURN(LoadMaterialsFromMTL(strMaterialFilename.c_str()));
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
DWORD CMeshLoader::AddVertex(UINT hash, VERTEX* pVertex)
{
	// If this vertex doesn't already exist in the Vertices list, create a new entry.
	// Add the index of the vertex to the Indices list.
	bool bFoundInList = false;
	DWORD index = 0;

	// Since it's very slow to check every element in the vertex list, a hashtable stores
	// vertex indices according to the vertex position's index as reported by the OBJ file
	if ((UINT)m_VertexCache.GetSize() > hash)
	{
		CacheEntry* pEntry = m_VertexCache.GetAt(hash);
		while (pEntry != NULL)
		{
			VERTEX* pCacheVertex = m_Vertices.GetData() + pEntry->index;

			// If this vertex is identical to the vertex already in the list, simply
			// point the index buffer to the existing vertex
			if (0 == memcmp(pVertex, pCacheVertex, sizeof(VERTEX)))
			{
				bFoundInList = true;
				index = pEntry->index;
				break;
			}

			pEntry = pEntry->pNext;
		}
	}

	// Vertex was not found in the list. Create a new entry, both within the Vertices list
	// and also within the hashtable cache
	if (!bFoundInList)
	{
		// Add to the Vertices list
		index = m_Vertices.GetSize();
		m_Vertices.Add(*pVertex);

		// Add this to the hashtable
		CacheEntry* pNewEntry = new CacheEntry;
		if (pNewEntry == NULL)
			return (DWORD)-1;

		pNewEntry->index = index;
		pNewEntry->pNext = NULL;

		// Grow the cache if needed
		while ((UINT)m_VertexCache.GetSize() <= hash)
		{
			m_VertexCache.Add(NULL);
		}

		// Add to the end of the linked list
		CacheEntry* pCurEntry = m_VertexCache.GetAt(hash);
		if (pCurEntry == NULL)
		{
			// This is the head element
			m_VertexCache.SetAt(hash, pNewEntry);
		}
		else
		{
			// Find the tail
			while (pCurEntry->pNext != NULL)
			{
				pCurEntry = pCurEntry->pNext;
			}

			pCurEntry->pNext = pNewEntry;
		}
	}

	return index;
}


//--------------------------------------------------------------------------------------
void CMeshLoader::DeleteCache()
{
	// Iterate through all the elements in the cache and subsequent linked lists
	for (int i = 0; i < m_VertexCache.GetSize(); i++)
	{
		CacheEntry* pEntry = m_VertexCache.GetAt(i);
		while (pEntry != NULL)
		{
			CacheEntry* pNext = pEntry->pNext;
			SAFE_DELETE(pEntry);
			pEntry = pNext;
		}
	}

	m_VertexCache.RemoveAll();
}


//--------------------------------------------------------------------------------------
HRESULT CMeshLoader::LoadMaterialsFromMTL(const WCHAR* strFileName)
{
	HRESULT hr;

	// Set the current directory based on where the mesh was found
	WCHAR wstrOldDir[MAX_PATH] = { 0 };
	GetCurrentDirectory(MAX_PATH, wstrOldDir);
	SetCurrentDirectory(m_strMediaDir);

	// Find the file
	WCHAR strPath[MAX_PATH];
	char cstrPath[MAX_PATH];
	V_RETURN(DXUTFindDXSDKMediaFileCch(strPath, MAX_PATH, strFileName));
	WideCharToMultiByte(CP_ACP, 0, strPath, -1, cstrPath, MAX_PATH, NULL, NULL);

	wstring filestr = strFileName;
	int size = filestr.rfind(L"/");
	filestr = filestr.substr(0, size);

	vector<Material*> *temp;
	temp = MESHMANAGER->AddMaterialList(filestr, m_Materials);

	if (temp != nullptr)
	{
		for (auto iter : *m_Materials)
		{
			SAFE_DELETE(iter);
		}
		m_Materials->clear();
		m_Materials = temp;
		isCopyMat = true;
		return S_OK;
	}

	// File input
	WCHAR strCommand[256] = { 0 };
	wifstream InFile(cstrPath);
	if (!InFile)
		return DXTRACE_ERR(L"wifstream::open", E_FAIL);

	// Restore the original current directory
	SetCurrentDirectory(wstrOldDir);

	Material* pMaterial = NULL;

	for (; ; )
	{
		InFile >> strCommand;
		if (!InFile)
			break;

		if (0 == wcscmp(strCommand, L"newmtl"))
		{
			// Switching active materials
			WCHAR strName[MAX_PATH] = { 0 };
			InFile >> strName;

			pMaterial = NULL;
			for (int i = 0; i < m_Materials->size(); i++)
			{
				Material* pCurMaterial = (*m_Materials)[i];
				if (0 == wcscmp(pCurMaterial->strName, strName))
				{
					pMaterial = pCurMaterial;
					break;
				}
			}
		}

		// The rest of the commands rely on an active material
		if (pMaterial == NULL)
			continue;

		if (0 == wcscmp(strCommand, L"#"))
		{
			// Comment
		}
		else if (0 == wcscmp(strCommand, L"Ka"))
		{
			// Ambient color
			float r, g, b;
			InFile >> r >> g >> b;
			pMaterial->mat.Ambient = D3DXCOLOR(r, g, b, 1);
		}
		else if (0 == wcscmp(strCommand, L"Kd"))
		{
			// Diffuse color
			float r, g, b;
			InFile >> r >> g >> b;
			pMaterial->mat.Diffuse = D3DXCOLOR(r, g, b, 1);
		}
		else if (0 == wcscmp(strCommand, L"Ks"))
		{
			// Specular color
			float r, g, b;
			InFile >> r >> g >> b;
			pMaterial->mat.Specular = D3DXCOLOR(r, g, b, 1);
		}
		else if (0 == wcscmp(strCommand, L"d"))
		{
			// Alpha
			InFile >> pMaterial->fAlpha;
		}
		else if (0 == wcscmp(strCommand, L"Ns"))
		{
			// Shininess
			int nShininess;
			InFile >> nShininess;
			pMaterial->nShininess = nShininess;
		}
		else if (0 == wcscmp(strCommand, L"illum"))
		{
			// Specular on/off
			int illumination;
			InFile >> illumination;
			pMaterial->bSpecular = (illumination == 2);
		}
		else if (0 == wcscmp(strCommand, L"map_Kd"))
		{
			// Texture
			WCHAR wstr[128];
			InFile >> wstr;
			pMaterial->strTexture = wstr;
		}

		else
		{
			// Unimplemented or unrecognized command
		}

		InFile.ignore(1000, L'\n');
	}

	InFile.close();

	return S_OK;
}


//--------------------------------------------------------------------------------------
void CMeshLoader::InitMaterial(Material* pMaterial)
{
	ZeroMemory(pMaterial, sizeof(Material));

	pMaterial->mat.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1);
	pMaterial->mat.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1);
	pMaterial->mat.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1);
	pMaterial->nShininess = 0;
	pMaterial->fAlpha = 1.0f;
	pMaterial->bSpecular = false;
	pMaterial->pTexture = NULL;
}

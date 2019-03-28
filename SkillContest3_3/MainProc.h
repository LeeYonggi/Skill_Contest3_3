#pragma once
class MainProc
{
public:
	MainProc();
	virtual ~MainProc();

private:
	CMeshLoader *mesh;
public:
	void Init();
	void Update();
	void Render();
	void Release();
};


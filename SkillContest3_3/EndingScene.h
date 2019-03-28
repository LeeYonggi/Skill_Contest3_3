#pragma once
#include "Scene.h"
class EndingScene :
	public Scene
{
public:
	EndingScene(bool _isHappy);
	virtual ~EndingScene();

private:
	bool isHappy;
	Texture *texture;
	vector<Texture*> animeTex;
	Texture *score;
	Texture *nameTex;
	float frame;
	string name;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
};


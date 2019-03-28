#pragma once
#include "Scene.h"

struct Acount
{
	string name;
	int point;
	Acount(string _name, int _point)
		: name(_name), point(_point)
	{

	}
};
class UI;
class MainScene :
	public Scene
{
public:
	MainScene();
	virtual ~MainScene();

private:
	Texture * info;
	Texture *howto;
	Texture *rank;
	Texture *title;
	vector<UI*> vUI;
	string soundKey;
	float frame;

public:
	bool isInfo		= false;
	bool isHowto	= false;
	bool isRank		= false;
	bool endIntro   = false;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
};


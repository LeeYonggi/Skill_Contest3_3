#pragma once
#include "Object.h"
class Background :
	public Object
{
public:
	Background(STAGE_STATE _stage);
	virtual ~Background();

private:
	STAGE_STATE stage;

public:
	Texture *minimap1;
	Texture *minimap2;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
};


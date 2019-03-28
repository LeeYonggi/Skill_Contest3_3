#pragma once
#include "Object.h"


class ObjectCreater :
	public Object
{
public:
	ObjectCreater(STAGE_STATE _stage);
	virtual ~ObjectCreater();

private:
	STAGE_STATE stage;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
};


#pragma once
#include "Object.h"

enum ITEM_KIND
{
	ITEM_JUMP,
	ITEM_SPEEDUP,
	ITEM_LENGTHUP,
	ITEM_THIRD,
	ITEM_TRAKINGMISSILE,
	ITEM_NUCLEAR
};
class Item :
	public Object
{
public:
	Item(Vector3 _pos, ITEM_KIND kind);
	virtual ~Item();

public:
	ITEM_KIND itemKind;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override; 
	virtual void Release()	override;
};


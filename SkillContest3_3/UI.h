#pragma once
#include "Object.h"

#include <functional>
class UI :
	public Object
{
public:
	UI(bool _isButton, Vector2 _pos, Vector2 _size, string idle, string down, function<void()> _func);
	UI(Vector2 _pos, string str, int low, int high, bool _iselaps, float _frameDelay);
	virtual ~UI();

private:
	function<void()> func;
	Texture *idle;
	Texture *down;
	Vector2 size;
	bool isButton;
	bool isDown;
	bool iselaps;
	float frameDelay;
public:
	bool animeEnd;

public:
	virtual void Init()		override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;

public:
	bool GetRectCollision(Vector2 p1, Vector2 _size);
};


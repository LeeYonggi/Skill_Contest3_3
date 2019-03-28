#include "DXUT.h"
#include "UI.h"


UI::UI(bool _isButton, Vector2 _pos, Vector2 _size, string idle, string down, function<void()> _func)
{
	isButton = _isButton;
	pos.x = _pos.x;
	pos.y = _pos.y;
	size = _size;
	func = _func;
	this->idle = IMAGEMANAGER->AddTexture(idle, idle);
	this->down = IMAGEMANAGER->AddTexture(down, down);
}

UI::UI(Vector2 _pos, string str, int low, int high, bool _iselaps, float _frameDelay)
{
	pos.x = _pos.x;
	pos.y = _pos.y;
	size = { 0, 0 };
	isButton = false;
	animeTexture.push_back(IMAGEMANAGER->AddAnimeTexture(str, str, low, high));
	iselaps = _iselaps;
	frameDelay = _frameDelay;
}


UI::~UI()
{
}

void UI::Init()
{
	isDown = false;
	animeEnd = false;
}

void UI::Update()
{
	if (isButton)
	{
		Vector2 temp;
		temp.x = pos.x;
		temp.y = pos.y;
		if (GetRectCollision(temp, size))
		{
			isDown = true;
			if (INPUTMANAGER->KeyDown(VK_LBUTTON))
			{
				func();
			}
		}
		else
		{
			isDown = false;
		}
	}
	else
	{
		if (iselaps)
		{
			frame += ELTIME;
		}
		else
			frame += 0.013;
	}
}

void UI::Render()
{
	if (isButton)
	{
		if (isDown)
			IMAGEMANAGER->DrawTexture(down, { pos.x, pos.y });
		else
			IMAGEMANAGER->DrawTexture(idle, {pos.x, pos.y});
	}
	else
	{
		int count = int(frame * frameDelay) % animeTexture[0].size();
		if (count >= animeTexture[0].size() - 1)
			animeEnd = true;
		IMAGEMANAGER->DrawTexture(animeTexture[0][count], { pos.x, pos.y });
	}
}

void UI::Release()
{

}

bool UI::GetRectCollision(Vector2 p1, Vector2 _size)
{
	RECT re = { 0 };
	RECT re1 = { int(p1.x - _size.x * 0.5), int(p1.y - _size.y * 0.5), 
		int(p1.x + _size.x * 0.5), int(p1.y + _size.y * 0.5) };
	POINT mouse;
	mouse.x = INPUTMANAGER->GetMouse().x;
	mouse.y = INPUTMANAGER->GetMouse().y;
	RECT re2 = { mouse.x, mouse.y, mouse.x + 1, mouse.y + 1 };
	return (IntersectRect(&re, &re1, &re2));
}

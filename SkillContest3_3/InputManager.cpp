#include "DXUT.h"
#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::Init()
{
	memset(nowKey, false, sizeof(nowKey));
	memset(pastKey, false, sizeof(pastKey));
	mouse = { 0, 0 };
}

void InputManager::Update()
{
	memcpy(pastKey, nowKey, sizeof(nowKey));
	for (int i = 0; i < MAX_KEY; ++i)
		nowKey[i] = GetAsyncKeyState(i) & 0x8000;

	POINT point;

	GetCursorPos(&point);
	ScreenToClient(DXUTGetHWND(), &point);

	mouse.x = point.x;
	mouse.y = point.y;
}

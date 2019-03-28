#pragma once
#include "Singleton.h"

#define MAX_KEY 256
class InputManager :
	public Singleton<InputManager>
{
public:
	InputManager();
	virtual ~InputManager();

private:
	bool nowKey[MAX_KEY];
	bool pastKey[MAX_KEY];
	Vector2 mouse;

public:
	void Init();
	void Update();

public:
	bool KeyDown(int key) { return (nowKey[key] == true && pastKey[key] == false); }
	bool KeyPress(int key) { return (nowKey[key] == true && pastKey[key] == true); }
	bool KeyUp(int key) { return (nowKey[key] == false && pastKey[key] == true); }
	Vector2 GetMouse() { return mouse; }
};

#define INPUTMANAGER InputManager::GetInstance()
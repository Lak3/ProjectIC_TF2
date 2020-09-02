#pragma once
#include "../../../SDK/Includes/Includes.h"

enum struct EKeyState {
	NONE,
	PRESSED,
	HELD
};

enum struct EScrollWheelState {
	NONE,
	UP,
	DOWN
};

class C_Input
{
public:
	std::map<int, EKeyState> m_Keyboard = {};	
	EKeyState m_RMouse					= {};
	EKeyState m_LMouse					= {};
	EKeyState m_MMouse					= {};
	EScrollWheelState m_Wheel			= {};

	void Update();

	EKeyState GetKey(int key);
	EKeyState GetMouse(int key);
};

extern C_Input gInput;
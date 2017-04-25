#pragma once
/////////////////////////////////////
// KeyboardDevice.h
// Maintain Mouse.
/////////////////////////////////////

#ifndef _MOUSE_DEVICE_H_
#define _MOUSE_DEVICE_H_

// Include
#include"InputDevice.h"

// Mouse Class
class MouseDevice :public InputDevice
{
public:
	MouseDevice();
	MouseDevice(const MouseDevice&);
	~MouseDevice();

	bool Initialize();
	bool Update();

	// Current state of mouse
	DIMOUSESTATE m_curState;
	// Previous state of mouse
	DIMOUSESTATE m_preState;

	// Postion of mouse
	int m_posX, m_posY;
};

#endif // !_MOUSE_DEVICE_H_

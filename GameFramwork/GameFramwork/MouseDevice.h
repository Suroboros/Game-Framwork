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
	DIMOUSESTATE m_mouseState;
};

#endif // !_MOUSE_DEVICE_H_

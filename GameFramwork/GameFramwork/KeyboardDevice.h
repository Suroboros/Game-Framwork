#pragma once
/////////////////////////////////////
// KeyboardDevice.h
// Maintain Keyboard.
/////////////////////////////////////

#ifndef _KEYBOARD_DEVICE_H_
#define _KEYBOARD_DEVICE_H_

// Include
#include"InputDevice.h"

// Keyboard Class
class KeyboardDevice :public InputDevice
{
public:
	KeyboardDevice();
	KeyboardDevice(const KeyboardDevice&);
	~KeyboardDevice();

	bool Initialize();
	bool Update();

	// Current state of keyboard
	unsigned char m_curState[256];
	// Previous state of keyboard
	unsigned char m_preState[256];
};

#endif // !_KEYBOARD_DEVICE_H_
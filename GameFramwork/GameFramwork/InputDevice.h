#pragma once
/////////////////////////////////////
// InputDevice.h
// Maintain Input Device.
/////////////////////////////////////

#ifndef _INPUT_DEVICE_H_
#define _INPUT_DEVICE_H_

// Linking
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// Inludes
#include<dinput.h>

// Input device class
class InputDevice
{
public:
	// Device type
	enum DeviceType
	{
		UNKNOW,
		KEYBOARD,
		MOUSE,
		JOYSTIC
	};

	// State of key and mouse button
	enum State
	{
		ON,
		OFF,
		DOWN,
		PRESS,
		UP
	};

	InputDevice();
	InputDevice(const InputDevice&);
	~InputDevice();

	virtual bool Initialize() = 0;
	void Shutdown();
	virtual bool Update() = 0;

	IDirectInputDevice8* m_inputDevice;

	DeviceType m_deviceType;

	// Current state of key or mouse button
	State m_curState;
	// Previous state of key or mouse button
	State m_preState;

};

#endif // !_INPUT_DEVICE_H_
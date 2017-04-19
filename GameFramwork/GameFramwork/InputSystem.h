#pragma once
/////////////////////////////////////
// InputSystem.h
// Input system.
/////////////////////////////////////

#ifndef _INPUTSYSTEM_H_
#define _INPUTSYSTEM_H_

// Version of Direct input
#define DIRECTINPUT_VERSION 0x0800

// Linking
//#pragma comment(lib,"dinput8.lib")
//#pragma comment(lib, "dxguid.lib")

// Inludes
//#include<dinput.h>
#include "InputDevice.h"
#include "KeyboardDevice.h"
#include "MouseDevice.h"
#include <map>

// Input class
class InputSystem
{
public:
	

	bool Initialize();
	void Shutdown();
	bool Frame();

	// Get instance(Singleton)
	static InputSystem& GetInstance();

	// Get input
	IDirectInput8* GetInput();

	// Input devices
	map<string, InputDevice*>  m_inputs;

	// Create input device
	bool CreateKeyborad(string name);
	bool CreateMouse(string name);

	// Keybord
	bool IsEscapePressed(string name);
	bool IsKeyDown(string name,int key);
	
	// Mouse
	void GetMouseLocation(string name, int& x, int& y);
	bool IsLeftButtonDown(string name);
	bool IsRightButtonDown(string name);

private:

	InputSystem();
	InputSystem(const InputSystem&);
	~InputSystem();
	InputSystem& operator=(const InputSystem&);

	IDirectInput8* m_directInput;
};

#endif 

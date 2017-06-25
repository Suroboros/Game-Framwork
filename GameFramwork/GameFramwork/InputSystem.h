#pragma once
/////////////////////////////////////
// InputSystem.h
// Input system.
/////////////////////////////////////

#ifndef _INPUTSYSTEM_H_
#define _INPUTSYSTEM_H_

// Version of Direct input
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif // !DIRECTINPUT_VERSION

// Inludes
#include "InputDevice.h"
#include "KeyboardDevice.h"
#include "MouseDevice.h"
#include <map>

using namespace std;

// Input class
class InputSystem
{
public:
	

	bool Initialize();
	void Shutdown();
	bool Frame();

	// Get instance of InputSystem(Singleton)
	static InputSystem& GetInstance();

	// Get input
	IDirectInput8* GetInput();



	// Create input device
	bool CreateKeyborad(string name);
	bool CreateMouse(string name);

	// Keybord
	bool IsEscapeDown(string name);
	bool IsKeyDown(string name,int key);
	bool IsKeyPressed(string name, int key);
	bool IsKeyUp(string name, int key);
	
	// Mouse
	void GetMouseLocation(string name, int& x, int& y);
	bool IsLeftButtonDown(string name);
	bool IsRightButtonDown(string name);

private:

	InputSystem();
	InputSystem(const InputSystem&);
	~InputSystem();
	InputSystem& operator=(const InputSystem&);

	// Input devices
	map<string, InputDevice*>  m_inputs;

	IDirectInput8* m_directInput;
};

#endif 

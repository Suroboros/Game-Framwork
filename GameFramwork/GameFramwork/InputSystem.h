#pragma once
/////////////////////////////////////
// InputSystem.h
// Input system.
/////////////////////////////////////

#ifndef _INPUTSYSTEM_H_
#define _INPUTSYSTEM_H_

// Inludes


// Input class
class InputSystem
{
public:
	InputSystem();
	InputSystem(const InputSystem&);
	~InputSystem();

	void Initialize();

	void KeyDown(unsigned int input);
	void KeyUp(unsigned int input);

	bool IsKeyDown(unsigned int input);

private:
	bool keys[256];
};

#endif 

/////////////////////////////////////
// InputSystem.cpp
// Input system.
/////////////////////////////////////

// Inludes
#include "InputSystem.h"

InputSystem::InputSystem()
{
}

InputSystem::InputSystem(const InputSystem &)
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::Initialize()
{
	// Initialize all keys.
	for (int i = 0;i < 256;i++)
	{
		keys[i] = false;
	}
}

void InputSystem::KeyDown(unsigned int input)
{
	// If a key is pressed then save the state in the key array.
	keys[input] = true;
}

void InputSystem::KeyUp(unsigned int input)
{
	// If a key is released then save the state in the key array.
	keys[input] = false;
}

bool InputSystem::IsKeyDown(unsigned int input)
{
	// Check the state of key.
	return keys[input];
}

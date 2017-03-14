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

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

private:
	bool keys[256];
};

#endif 

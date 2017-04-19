/////////////////////////////////////
// InputSystem.cpp
// Input system.
/////////////////////////////////////

// Inludes
#include "InputSystem.h"
#include "WinMain.h"

InputSystem::InputSystem()
{
	m_directInput = nullptr;
}

InputSystem::InputSystem(const InputSystem &)
{
}

InputSystem::~InputSystem()
{
}

InputSystem & InputSystem::operator=(const InputSystem &)
{
	//return *this;
}

bool InputSystem::Initialize()
{
	HRESULT hr;

	// Create the interface of Direct input
	hr = DirectInput8Create(WindowMain::GetInstance().GetHINSTSANCE(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, nullptr);
	if(FAILED(hr))
		return false;

	return true;
}

void InputSystem::Shutdown()
{
	// Release input devices
	for(auto input : m_inputs)
	{
		input.second->Shutdown();
		input.second = nullptr;
	}
	m_inputs.clear();
	
	// Release Direct input
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = nullptr;
	}
}

bool InputSystem::Frame()
{
	bool result;

	for(auto input : m_inputs){
		if(input.second){
			// Update current state of the input device
			result = input.second->Update();
			if(!result)
				return false;
		}
	}
	
	return true;
}

bool InputSystem::CreateKeyborad(string name)
{
	KeyboardDevice* keyboard = new KeyboardDevice;
	bool result = keyboard->Initialize();
	if(!result)
		return false;

	m_inputs.insert(pair<string, InputDevice*>(name, keyboard));

	return true;
}

bool InputSystem::CreateMouse(string name)
{
	MouseDevice* mouse = new MouseDevice;
	bool result = mouse->Initialize();
	if(!result)
		return false;

	m_inputs.insert(pair<string, InputDevice*>(name, mouse));

	return true;
}

bool InputSystem::IsEscapePressed(string name)
{
	auto input = m_inputs.find(name);

	if(input!=m_inputs.end()&&input->second->m_deviceType==InputDevice::DeviceType::KEYBOARD){
		if(dynamic_cast<KeyboardDevice*>(input->second)->m_keyboardState[DIK_ESCAPE] & 0x80)
			return true;
	}
	return false;
}

bool InputSystem::IsKeyDown(string name, int key)
{
	auto input = m_inputs.find(name);

	if(input != m_inputs.end() && input->second->m_deviceType == InputDevice::DeviceType::KEYBOARD) {
		if(dynamic_cast<KeyboardDevice*>(input->second)->m_keyboardState[key] & 0x80)
			return true;
	}
	return false;
}

void InputSystem::GetMouseLocation(string name, int & x, int & y)
{
	auto input = m_inputs.find(name);

	if(input != m_inputs.end() && input->second->m_deviceType == InputDevice::DeviceType::MOUSE) {
		auto input = m_inputs.find(name);
		x = dynamic_cast<MouseDevice*>(input->second)->m_mouseState.lX;
		y = dynamic_cast<MouseDevice*>(input->second)->m_mouseState.lY;
	}
}

bool InputSystem::IsLeftButtonDown(string name)
{
	auto input = m_inputs.find(name);

	if(input != m_inputs.end() && input->second->m_deviceType == InputDevice::DeviceType::MOUSE) {
		auto input = m_inputs.find(name);
		if(dynamic_cast<MouseDevice*>(input->second)->m_mouseState.rgbButtons[0] & 0x80)
			return true;
	}
	return false;
}

bool InputSystem::IsRightButtonDown(string name)
{
	auto input = m_inputs.find(name);

	if(input != m_inputs.end() && input->second->m_deviceType == InputDevice::DeviceType::MOUSE) {
		auto input = m_inputs.find(name);
		if(dynamic_cast<MouseDevice*>(input->second)->m_mouseState.rgbButtons[1] & 0x80)
			return true;
	}
	return false;
}

InputSystem & InputSystem::GetInstance()
{
	static InputSystem instance;
	return instance;
}

IDirectInput8 * InputSystem::GetInput()
{
	return m_directInput;
}

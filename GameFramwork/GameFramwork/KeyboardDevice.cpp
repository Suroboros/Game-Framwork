#include "KeyboardDevice.h"
#include "InputSystem.h"
#include "WinMain.h"

KeyboardDevice::KeyboardDevice()
{
}

KeyboardDevice::KeyboardDevice(const KeyboardDevice &)
{
}

KeyboardDevice::~KeyboardDevice()
{
}

bool KeyboardDevice::Initialize()
{
	HRESULT hr;

	// Create mouse interface
	hr = InputSystem::GetInstance().GetInput()->CreateDevice(GUID_SysKeyboard, &m_inputDevice, nullptr);
	if(FAILED(hr))
		return false;

	// Set the data fromat
	hr = m_inputDevice->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
		return false;

	// Set the cooperative level to not share with other program
	hr = m_inputDevice->SetCooperativeLevel(WindowMain::GetInstance().GetHwnd(), DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if(FAILED(hr))
		return false;

	// Acquire the mouse
	hr = m_inputDevice->Acquire();
	if(FAILED(hr))
		return false;
	// Set device type
	m_deviceType = DeviceType::KEYBOARD;

	return true;
}

bool KeyboardDevice::Update()
{
	HRESULT hr;

	// Read keyboard device
	hr = m_inputDevice->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if(FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired, try to get control back
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_inputDevice->Acquire();
		}
		else
			return false;
	}
	return true;
}

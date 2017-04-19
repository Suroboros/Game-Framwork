#include "MouseDevice.h"
#include "InputSystem.h"
#include "WinMain.h"

MouseDevice::MouseDevice()
{
}

MouseDevice::MouseDevice(const MouseDevice &)
{
}

MouseDevice::~MouseDevice()
{
}

bool MouseDevice::Initialize()
{
	HRESULT hr;

	// Create mouse interface
	hr = InputSystem::GetInstance().GetInput()->CreateDevice(GUID_SysMouse, &m_inputDevice, nullptr);
	if(FAILED(hr))
		return false;

	// Set the data fromat
	hr = m_inputDevice->SetDataFormat(&c_dfDIMouse);
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
	m_deviceType = DeviceType::MOUSE;

	return true;
}

bool MouseDevice::Update()
{
	HRESULT hr;

	// Read mouse device
	hr = m_inputDevice->GetDeviceState(sizeof(m_mouseState), (LPVOID)&m_mouseState);
	if(FAILED(hr))
	{
		// If the mouse lost focus or was not acquired, try to get control back
		if((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_inputDevice->Acquire();
		}
		else
			return false;
	}
	return true;
}

#include "MouseDevice.h"
#include "InputSystem.h"
#include "WinMain.h"

MouseDevice::MouseDevice()
{
	ZeroMemory(&m_curState, sizeof(m_curState));
	ZeroMemory(&m_preState, sizeof(m_preState));
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

	// Store previous state
	std::memcpy(&m_preState, &m_curState, sizeof(m_curState));

	// Read mouse device
	hr = m_inputDevice->GetDeviceState(sizeof(m_curState), (LPVOID)&m_curState);
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

	// Update the position of mouse
	m_posX += m_curState.lX;
	m_posY += m_curState.lY;

	// Ensure the mouse location doesn`t exceed the screen width or height
	if(m_posX < 0)m_posX = 0;
	if(m_posY < 0)m_posY = 0;
	if(m_posX > WindowMain::GetInstance().GetScreenWidth())
		m_posX = WindowMain::GetInstance().GetScreenWidth();
	if(m_posY > WindowMain::GetInstance().GetScreenHeight())
		m_posY = WindowMain::GetInstance().GetScreenHeight();

	return true;
}

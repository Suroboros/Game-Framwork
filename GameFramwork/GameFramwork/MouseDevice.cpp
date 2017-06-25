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

	// Set the cooperative level
	hr = m_inputDevice->SetCooperativeLevel(WindowMain::GetInstance().GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
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
	
	// Get mouse position.
	GetCursorPos(&m_pos);
	// Convert screen position to client window position.
	ScreenToClient(WindowMain::GetInstance().GetHwnd(), &m_pos);
	// Get client size
	RECT windowRect;
	GetClientRect(WindowMain::GetInstance().GetHwnd(), &windowRect);
	// Fix mouse position. FixedPos = UnfixedPos * backBufferSize / clientSize
	m_pos.x = m_pos.x*static_cast<LONG>(static_cast<float>(WindowMain::GetInstance().GetScreenWidth()) / static_cast<float>(windowRect.right - windowRect.left));
	m_pos.y = m_pos.y*static_cast<LONG>(static_cast<float>(WindowMain::GetInstance().GetScreenHeight()) / static_cast<float>(windowRect.bottom - windowRect.top));

	return true;
}

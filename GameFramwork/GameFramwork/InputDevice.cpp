#include "InputDevice.h"

InputDevice::InputDevice()
{
	m_inputDevice = nullptr;
	m_curState = State::OFF;
	m_preState = State::OFF;
}

InputDevice::InputDevice(const InputDevice &)
{
}

InputDevice::~InputDevice()
{
}

void InputDevice::Shutdown()
{
	if(m_inputDevice)
	{
		m_inputDevice->Unacquire();
		m_inputDevice->Release();
		m_inputDevice = nullptr;
	}
}

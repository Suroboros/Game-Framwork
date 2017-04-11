/////////////////////////////////////
// Light.cpp
// Maintain the light(color and direction)
/////////////////////////////////////
#include "Light.h"

Light::Light()
{
	m_color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Light::Light(const Light & light)
{
}

Light::~Light()
{
}

void Light::SetColor(float r, float g, float b, float a)
{
	m_color = XMFLOAT4(r, g, b, a);
	return;
}

void Light::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

XMFLOAT4 Light::GetColor()
{
	return m_color;
}

XMFLOAT3 Light::GetDirection()
{
	return m_direction;
}

/////////////////////////////////////
// Light.cpp
// Maintain the light(color and direction)
/////////////////////////////////////
#include "Light.h"

Light::Light()
{
	color = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

Light::Light(const Light & light)
{
}

Light::~Light()
{
}

void Light::SetColor(float r, float g, float b, float a)
{
	color = XMFLOAT4(r, g, b, a);
	return;
}

void Light::SetDirection(float x, float y, float z)
{
	direction = XMFLOAT3(x, y, z);
	return;
}

XMFLOAT4 Light::GetColor()
{
	return color;
}

XMFLOAT3 Light::GetDirection()
{
	return direction;
}

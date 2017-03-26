#pragma once
/////////////////////////////////////
// Light.h
// Maintain the light(color and direction)
/////////////////////////////////////

#ifndef _LIGHT_H_
#define _LIGHT_H_

// Include
#include <DirectXMath.h>

using namespace DirectX;
// Light Class
class Light
{
	
public:
	Light();
	Light(const Light& light);
	~Light();

	

	void SetColor(float r, float g, float b, float a);
	void SetDirection(float x, float y, float z);

	XMFLOAT4 GetColor();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4 color;
	XMFLOAT3 direction;

};

#endif // !_LIGHT_H_
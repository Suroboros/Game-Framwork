#pragma once
/////////////////////////////////////
// GEUtility.h
// Utility tools
/////////////////////////////////////

#ifndef _GEUTILITY_H_
#define _GEUTILITY_H_

// Include
#include <DirectXMath.h>
using namespace DirectX;

float ToRadian(const float);
float ToDegree(const float);

// Point struct
struct Point
{
	float x;
	float y;
	float z;
};

// Rectangle struct
struct Rect
{
	float top;
	float left;
	float bottom;
	float right;
};

#endif // !_GEUTILITY_H_
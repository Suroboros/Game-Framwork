#pragma once
/////////////////////////////////////
// GEUtility.h
// Utility tools
/////////////////////////////////////

#ifndef _GEUTILITY_H_
#define _GEUTILITY_H_

// Include
#include <DirectXMath.h>
#include <tchar.h>

using namespace DirectX;
using namespace std;

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

// Color struct
struct Color
{
	float r;
	float g;
	float b;
	float a;
};

#endif // !_GEUTILITY_H_
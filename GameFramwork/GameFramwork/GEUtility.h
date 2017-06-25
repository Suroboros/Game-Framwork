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
	float left;
	float top;
	float width;
	float height;
};

// Box struct
struct Box
{
	// The center of box
	Point center;

	float width;
	float height;
	float depth;
};

// Color struct
struct Color
{
	float r;
	float g;
	float b;
	float a;
};

// Change degree to radian
float ToRadian(const float degree);
// Change radian to degree
float ToDegree(const float radian);

// Transpose
// Rect transpose
void Transpose(Rect& r, float x, float y);
// Box transpose
void Transpose(Box& b, float x, float y, float z);

// The distance from a point to a plane
float Point2PlaneDis(XMFLOAT4 plant, Point p);

// Collision Check
// Two rectangle
bool CollisionCheck(const Rect r1, const Rect r2);
// Two box
bool CollisionCheck(const Box b1, const Box b2);
#endif // !_GEUTILITY_H_
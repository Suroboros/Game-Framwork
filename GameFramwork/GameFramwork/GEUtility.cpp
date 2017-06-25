/////////////////////////////////////
// GEUtility.h
// Utility tools
/////////////////////////////////////
#include "GEUtility.h"
#include <atlstr.h>
#include <cmath>

float ToRadian(const float degree)
{
	return degree*XM_PI / 180.0f;
}

float ToDegree(const float radian)
{
	return radian*180.0f / XM_PI;
}

void Transpose(Rect & r, float x, float y)
{
	r.left += x;
	r.top += y;
}

void Transpose(Box & b, float x, float y, float z)
{
	b.center.x += x;
	b.center.y += y;
	b.center.z += z;
}

float Point2PlaneDis(XMFLOAT4 plant, Point p)
{
	float output;
	output = plant.x*p.x + plant.y*p.y + plant.z*p.z + plant.w*1.0f;
	
	return output;
}

bool CollisionCheck(const Rect r1, const Rect r2)
{
	if(r1.left<(r2.left + r2.width) && (r1.left + r1.width)>r2.left&&r1.top<(r2.top + r2.height) && (r1.top + r1.height)>r2.top)
		return true;
	else
		return false;
}

bool CollisionCheck(const Box b1, const Box b2)
{
	if(abs(b1.center.x-b2.center.x)<((b1.width + b2.width)/2.0f) || abs(b1.center.y - b2.center.y)<((b1.height + b2.height) / 2.0f) || abs(b1.center.z - b2.center.z)<((b1.depth + b2.depth) / 2.0f))
		return true;
	else
		return false;
}


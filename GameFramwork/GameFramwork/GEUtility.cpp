/////////////////////////////////////
// GEUtility.h
// Utility tools
/////////////////////////////////////
#include "GEUtility.h"

float ToRadian(const float degree)
{
	return degree*XM_PI / 180.0f;
}

float ToDegree(const float radian)
{
	return radian*180.0f / XM_PI;
}

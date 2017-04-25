#pragma once

#ifndef _OBJECT_H_
#define _OBJECT_H_

// Include
#include "GEUtility.h"

class Object
{
public:
	Object();
	Object(const Object&);
	~Object();

	bool Initialize();
	void Shutdown();

	void Render2D();
	void Render3D();
	void Update();

	Point pos;
	float speedX, speedY;
};

#endif // !_OBJECT_H_
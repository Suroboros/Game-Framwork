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
	virtual ~Object();

	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;

	virtual void Render2D() = 0;
	virtual void Render3D() = 0;
	virtual void Update() = 0;

	Point pos;
};

#endif // !_OBJECT_H_
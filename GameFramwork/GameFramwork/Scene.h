#pragma once
/////////////////////////////////////
// Scene.cpp
// Base scene class.
// Every scene is inherited from this class.
/////////////////////////////////////

#ifndef _SCENE_H_
#define _SCENE_H_

// Include

// Scene class
class Scene
{
public:
	Scene();
	Scene(const Scene&);
	~Scene();

	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;

	// Render for 2D.
	virtual void Render2D()=0;
	// Render for 3D.
	virtual void Render3D()=0;

	virtual void Update()=0;
};


#endif // !_SCENE_H_

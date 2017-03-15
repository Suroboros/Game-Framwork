/////////////////////////////////////
// GraphicSystem.h
// Graphic system.
/////////////////////////////////////

#ifndef _GRAPHICSYSTEM_H_
#define _GRAPHICSYSTEM_H_

// Inludes
#include <Windows.h>
#include "D3DClass.h"

/////////////
// GLOBALS //
/////////////
const bool FULLSCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

// Graphic class
class GraphicSystem
{
public:
	GraphicSystem();
	GraphicSystem(const GraphicSystem&);
	~GraphicSystem();

	bool Initialize(HWND hwnd, int screenWidth = 0, int screenHight = 0);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	D3DClass* D3DObject;

};

#endif 

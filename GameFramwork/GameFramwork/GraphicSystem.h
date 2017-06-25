#pragma once
/////////////////////////////////////
// GraphicSystem.h
// Graphic system.
/////////////////////////////////////

#ifndef _GRAPHICSYSTEM_H_
#define _GRAPHICSYSTEM_H_

// Inludes
#include <Windows.h>
#include "D3DClass.h"
#include "DWClass.h"
#include "D2DClass.h"
#include "GameEngine.h"

/////////////
// GLOBALS 
/////////////
static bool FULLSCREEN = false;
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

	bool Initialize();
	void Shutdown();
	bool Frame();

private:
	bool Render();

};

#endif 

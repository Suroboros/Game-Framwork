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
#include "Camera.h"
#include "Mesh.h"
#include "Light.h"
#include "LightShader.h"
#include "Image.h"
#include "TextureShader.h"

#pragma once
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

	bool Initialize();
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	//D3DClass* D3DObject;
	TextureShader* texShader;
	LightShader* lightShader;
	

public:
	Camera::spCamera camera[4]; //camera
	Model* model;
	Image* image;
	Light* light;
};

#endif 

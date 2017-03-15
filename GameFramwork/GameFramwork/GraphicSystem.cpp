/////////////////////////////////////
// GraphicSystem.cpp
// Graphic system.
/////////////////////////////////////

// Inludes
#include "GraphicSystem.h"

GraphicSystem::GraphicSystem()
{
	D3DObject = nullptr;
}

GraphicSystem::GraphicSystem(const GraphicSystem &)
{
}

GraphicSystem::~GraphicSystem()
{
}

bool GraphicSystem::Initialize(HWND hwnd, int screenWidth, int screenHight)
{
	bool result;

	// Create D3DClass object
	D3DObject = new D3DClass;
	if(!D3DObject)
	{
		return false;
	}

	// Initialize the D3DClass object
	result = D3DObject->Initialize(screenWidth, screenHight, VSYNC_ENABLED, hwnd, FULLSCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Can't initialize the Direct3D!", L"Error", MB_OK);
		DestroyWindow(hwnd);
		return false;
	}

	return true;
}

void GraphicSystem::Shutdown()
{
	//Release the Direct3D object
	if(D3DObject)
	{
		D3DObject->Shutdown();
		delete D3DObject;
		D3DObject = nullptr;
	}

	return;
}

bool GraphicSystem::Frame()
{
	bool result;

	//Render the graphics scene
	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicSystem::Render()
{

	//Clear the buffers to begin the scene
	D3DObject->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//Present the rendered scene to the screen
	D3DObject->EndScene();
	return true;
}

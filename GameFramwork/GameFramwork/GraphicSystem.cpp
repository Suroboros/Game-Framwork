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

	// Create default camera.
	camera[0] = Camera::Create(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), 
		ToRadian(60.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHight), 10.0f, 300.0f);
	camera[3] = camera[2] = camera[1] = camera[0];
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

	camera[0]->Update();

	//Present the rendered scene to the screen
	D3DObject->EndScene();
	return true;
}

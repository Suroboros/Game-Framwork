/////////////////////////////////////
// GraphicSystem.cpp
// Graphic system.
/////////////////////////////////////

// Inludes
#include "GraphicSystem.h"
#include "WinMain.h"

GraphicSystem::GraphicSystem()
{
}

GraphicSystem::GraphicSystem(const GraphicSystem &)
{
}

GraphicSystem::~GraphicSystem()
{
}

bool GraphicSystem::Initialize()
{
	bool result;

	// Create D3D object
	result = D3DClass::GetInstance().Initialize(VSYNC_ENABLED, FULLSCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Can't initialize Direct3D!"), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return false;
	}

	// Create D2D object
	result = D2DClass::GetInstance().Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Can't initialize Direct2D!"), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return false;
	}

	// Create direct write object
	result = DWClass::GetInstance().Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Can't initialize Direct write!"), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return false;
	}

	// Create Game engine obejct
	result = GameEngine::GetInstance().Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Can't initialize Game Egine!"), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return false;
	}
	
	return true;
}

void GraphicSystem::Shutdown()
{
	// Release the Game Engine object
	GameEngine::GetInstance().Shutdown();

	// Release the Direct2D object
	D2DClass::GetInstance().Shutdown();

	// Release the Direct3D object
	D3DClass::GetInstance().Shutdown();
}

bool GraphicSystem::Frame()
{
	bool result;

	GameEngine::GetInstance().Update();

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
	D3DClass::GetInstance().BeginScene(0.7f, 0.7f, 0.7f, 1.0f);

	// Render 3D
	GameEngine::GetInstance().Render3D();

	// Begin 2D rendring
	D2DClass::GetInstance().BeginDraw();

	// Render 2D
	GameEngine::GetInstance().Render2D();

	// End 2D rendering
	D2DClass::GetInstance().EndDraw();
	
	//Present the rendered scene to the screen
	D3DClass::GetInstance().EndScene();

	return true;
}

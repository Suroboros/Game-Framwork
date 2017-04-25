/////////////////////////////////////
// GraphicSystem.cpp
// Graphic system.
/////////////////////////////////////

// Inludes
#include "GraphicSystem.h"
#include "WinMain.h"

GraphicSystem::GraphicSystem()
{
	//D3DObject = nullptr;
	font = nullptr;
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
		DestroyWindow(WindowMain::GetInstance().GetHwnd());
		return false;
	}

	// Create D2D object
	result = D2DClass::GetInstance().Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Can't initialize Direct2D!"), _T("Error"), MB_OK);
		DestroyWindow(WindowMain::GetInstance().GetHwnd());
		return false;
	}

	// Create direct write object
	result = DWClass::GetInstance().Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Can't initialize Direct write!"), _T("Error"), MB_OK);
		DestroyWindow(WindowMain::GetInstance().GetHwnd());
		return false;
	}

	// Create Game engine obejct
	result = GameEngine::GetInstance().Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Can't initialize Game Egine!"), _T("Error"), MB_OK);
		DestroyWindow(WindowMain::GetInstance().GetHwnd());
		return false;
	}
	
	// Create font
	font = new Font;
	if(!font)
		return false;
	result = font->Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize the Font object"), _T("Error"), MB_OK);
		return false;
	}

	return true;
}

void GraphicSystem::Shutdown()
{
	// Realease the font object
	if(font)
	{
		font->Shutdown();
		delete font;
		font = nullptr;
	}

	// Release the Game Engine object
	GameEngine::GetInstance().Shutdown();

	// Release the Direct2D object
	D2DClass::GetInstance().Shutdown();

	// Release the Direct3D object
	D3DClass::GetInstance().Shutdown();


	return;
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
//	XMMATRIX world, view, projection;
//	bool result;

	//Clear the buffers to begin the scene
	D3DClass::GetInstance().BeginScene(0.7f, 0.7f, 0.7f, 1.0f);

	GameEngine::GetInstance().Render3D();

/*
	camera[0]->Update();

	// Get the world, view, and projection matrices
	camera[0]->GetViewMatrix(view);
	D3DClass::GetInstance().GetWorldMatrix(world);
	D3DClass::GetInstance().GetProjectionMatrix(projection);
	static float r = 0;
	r += (float)XM_PI*-0.005f;
	world = XMMatrixRotationY(r);

	// Render mesh
	//model->Render(D3DObject->GetDeviceContext());
	int i = model->GetIndexCount();
	// Render the model using the light shader
	for(auto mesh : model->m_model)
	{
		mesh->RenderBuffers();
		result = lightShader->Render(mesh, world, view, projection, model->GetTexture(), light->GetColor(), light->GetDirection(), camera[0]->GetViewDirection());
	}
	
	// Render image
	// Get projection matrix(ortho matrix)
	D3DClass::GetInstance().GetOrthoMatrix(projection);
	// Turn off depth buffer
	D3DClass::GetInstance().m_effect->DepthBufferOff();
	// Render image
	result = image->Render(100, 100);
	if(!result)
		return false;
	// Render image with shader
	result = texShader->Render(image->GetIndexCount(), world, view, projection, image->GetTexture());
	if(!result)
		return false;
	// Turn on depth buffer
	D3DClass::GetInstance().m_effect->DepthBufferOn();

*/
	// Start 2D rendering
	D2DClass::GetInstance().BeginDraw();
	GameEngine::GetInstance().Render2D();
/*	int x, y;
	InputSystem::GetInstance().GetMouseLocation("sysMouse", x, y);
	char text[256];
	char pos[16];
	strcpy_s(text, "Mouse Position:\nX:");
	_itoa_s(x, pos, 10);
	strcat_s(text, pos);
	strcat_s(text, "\nY:");
	_itoa_s(y, pos, 10);
	strcat_s(text, pos);
	TCHAR temp[256];
	USES_CONVERSION;
	_tcscpy_s(temp, A2T(text));
	font->CreateText(temp, _T("Arial"), 50.0, 1.0, 1.0, 1.0, 0.5);
*/
	// End 2D rendering
	D2DClass::GetInstance().EndDraw();
	
	//Present the rendered scene to the screen
	D3DClass::GetInstance().EndScene();

	return true;
}

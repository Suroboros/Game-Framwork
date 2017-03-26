/////////////////////////////////////
// GraphicSystem.cpp
// Graphic system.
/////////////////////////////////////

// Inludes
#include "GraphicSystem.h"

GraphicSystem::GraphicSystem()
{
	D3DObject = nullptr;
	model = nullptr;
	light = nullptr;
	lightShader = nullptr;
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
		MessageBox(hwnd, _T("Can't initialize the Direct3D!"), _T("Error"), MB_OK);
		DestroyWindow(hwnd);
		return false;
	}

	// Create default camera.
	float a = ToRadian(30.0f);
	camera[0] = Camera::Create(XMFLOAT3(0.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), 
		ToRadian(30.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHight), 10.0f, 300.0f);
	camera[3] = camera[2] = camera[1] = camera[0];

	// Create and initialize the mesh
	model = new Model;
	result = model->Initialize(D3DObject->GetDevice(), D3DObject->GetDeviceContext(), _T("./cube.obj"));
	if(!result)
	{
		MessageBox(hwnd, _T("Could not initialize the mesh object."), _T("Error"), MB_OK);
		return false;
	}

	// Create and initialize the light and light shader
	lightShader = new LightShader;
	if(!lightShader)
		return false;

	result = lightShader->Initialize(D3DObject->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, _T("Could not initialize the light shader object."), _T("Error"), MB_OK);
		return false;
	}

	light = new Light;
	if(!light)
		return false;

	light->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDirection(0.0f, 0.0f, 1.0f);

	return true;
}

void GraphicSystem::Shutdown()
{
	// Release the light object
	if(light)
	{
		delete light;
		light = nullptr;
	}

	// Release the light shader object
	if(lightShader)
	{
		lightShader->Shutdown();
		delete lightShader;
		lightShader = nullptr;
	}

	// Release the mesh object
	if(model)
	{
		model->Shutdown();
		delete model;
		model = nullptr;
	}

	// Release the Direct3D object
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
	XMMATRIX world, view, projection;
	bool result;

	//Clear the buffers to begin the scene
	D3DObject->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	camera[0]->Update();

	// Get the world, view, and projection matrices
	camera[0]->GetViewMatrix(view);
	D3DObject->GetWorldMatrix(world);
	D3DObject->GetProjectionMatrix(projection);
	static float r = 0;
	r += (float)XM_PI*-0.005f;
	world = XMMatrixRotationY(r);
	// Render mesh
	model->Render(D3DObject->GetDeviceContext());
	int i = model->GetIndexCount();
	// Render the model using the light shader
	for(auto mesh : model->model)
	{
		result = lightShader->Render(D3DObject->GetDeviceContext(), mesh->indices.size() , world, view, projection, model->GetTexture(), light->GetColor(), light->GetDirection());
	}
	//Present the rendered scene to the screen
	D3DObject->EndScene();
	return true;
}

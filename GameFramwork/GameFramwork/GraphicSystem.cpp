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
	model = nullptr;
	light = nullptr;
	lightShader = nullptr;
	image = nullptr;
	texShader = nullptr;
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


	result = D3DClass::GetInstance().Initialize(VSYNC_ENABLED, FULLSCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Can't initialize the Direct3D!"), _T("Error"), MB_OK);
		DestroyWindow(WindowMain::GetInstance().GetHwnd());
		return false;
	}

	// Create default camera.
	float a = ToRadian(30.0f);
	camera[0] = Camera::Create(XMFLOAT3(5.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), 
		ToRadian(120.0f), static_cast<float>(WindowMain::GetInstance().GetScreenWidth()) / static_cast<float>(WindowMain::GetInstance().GetScreenHeight()), 1.0f, 300.0f);
	camera[3] = camera[2] = camera[1] = camera[0];

	// Create and initialize the mesh
	model = new Model;
//	result = model->Initialize(D3DObject->GetDevice(), D3DObject->GetDeviceContext(), _T("./kuma.obj"));
	result = model->Initialize(_T("./kuma.obj"));
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize the mesh object."), _T("Error"), MB_OK);
		return false;
	}

	// Create and initialize the light and light shader
	lightShader = new LightShader;
	if(!lightShader)
		return false;

	result = lightShader->Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize the light shader object."), _T("Error"), MB_OK);
		return false;
	}

	light = new Light;
	if(!light)
		return false;

	light->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	light->SetDirection(0.0f, 0.0f, 1.0f);


	// Create and Initialize the texture shader
	texShader = new TextureShader;
	if(!texShader)
		return false;

	result = texShader->Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize the texture shader object"), _T("Error"), MB_OK);
		return false;
	}

	// Create and Initialize the image
	image = new Image();
	if(!image)
		return false;
	result = image->Initialize(_T("./tex.bmp"));
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize the image object"), _T("Error"), MB_OK);
		return false;
	}

	return true;
}

void GraphicSystem::Shutdown()
{
	// Release the image object
	if(image)
	{
		image->ShutDown();
		delete image;
		image = nullptr;
	}

	// Release the texutre shader
	if(texShader)
	{
		texShader->Shutdown();
		delete texShader;
		texShader = nullptr;
	}

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
	D3DClass::GetInstance().Shutdown();

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
	D3DClass::GetInstance().BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

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

	//Present the rendered scene to the screen
	D3DClass::GetInstance().EndScene();
	return true;
}

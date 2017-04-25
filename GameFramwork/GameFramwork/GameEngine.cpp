#include "GameEngine.h"
#include "GEUtility.h"
#include "WinMain.h"

GameEngine::GameEngine()
{
	world = XMMatrixIdentity();
}

GameEngine::~GameEngine()
{
}

GameEngine & GameEngine::operator=(const GameEngine &)
{
	return *this;
}

bool GameEngine::Initialize()
{
	bool result;

	// Create default camera.
	float a = ToRadian(30.0f);
	camera[0] = Camera::Create(XMFLOAT3(5.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f),
		ToRadian(120.0f), static_cast<float>(WindowMain::GetInstance().GetScreenWidth()) / static_cast<float>(WindowMain::GetInstance().GetScreenHeight()), 1.0f, 300.0f);
	camera[3] = camera[2] = camera[1] = camera[0];

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

	m_object = new Object;
	if(!m_object)
		return false;
	result = m_object->Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize object"), _T("Error"), MB_OK);
		return false;
	}
	return true;
}

void GameEngine::Shutdown()
{
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

	// Release model list
	for(auto model : modelList)
	{
		model.second->Shutdown();
		model.second = nullptr;

	}
	modelList.clear();

	// Release image list
	for(auto image : imageList)
	{
		image.second->ShutDown();
		image.second = nullptr;
	}
	imageList.clear();
	

	// Release object list
	if(m_object)
	{
		m_object->Shutdown();
		delete m_object;
		m_object = nullptr;
	}
}

GameEngine & GameEngine::GetInstance()
{
	static GameEngine instance;
	return instance;
}

bool GameEngine::CreateModel(string name, TCHAR* modePath, TCHAR* texPath)
{
	bool result;

	Model* model = new Model;
	if(!model)
		return false;

	result = model->Initialize(modePath, texPath);
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize the mesh object."), _T("Error"), MB_OK);
		return false;
	}
	modelList.insert(pair<string, Model*>(name, model));
	return true;
}

bool GameEngine::CreateImage(string name, TCHAR* path)
{
	bool result;

	Image* image = new Image;
	if(!image)
		return false;

	result = image->Initialize(path);
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize the image object."), _T("Error"), MB_OK);
		return false;
	}
	imageList.insert(pair<string, Image*>(name, image));
	return true;
}

void GameEngine::CreateText(TCHAR * text, TCHAR * font, float size, Color color, Rect box)
{
	Font::GetInstance().CreateText(text, font, size, color, box);
}

void GameEngine::DrawModel(string name)
{
	auto model = modelList.find(name);
	// Render mesh
	//model->Render(D3DObject->GetDeviceContext());
	int i = model->second->GetIndexCount();
	// Render the model using the light shader
	for(auto mesh : model->second->m_model)
	{
		mesh->RenderBuffers();
		lightShader->Render(mesh, world, view, projection, model->second->GetTexture(), light->GetColor(), light->GetDirection(), camera[0]->GetViewDirection());
	}
}

void GameEngine::DrawImage(string name)
{
	auto image = imageList.find(name);
	// Render image
	// Get projection matrix(ortho matrix)
	D3DClass::GetInstance().GetOrthoMatrix(projection);
	// Turn off depth buffer
	D3DClass::GetInstance().m_effect->DepthBufferOff();
	// Render image
	image->second->Render(100, 100);
	
	// Render image with shader
	texShader->Render(image->second->GetIndexCount(), world, view, projection, image->second->GetTexture());
	
	// Turn on depth buffer
	D3DClass::GetInstance().m_effect->DepthBufferOn();
}

void GameEngine::DrawCircle(float x, float y, float r)
{
	D2DClass::GetInstance().DrawCircle(x, y, r);
}

void GameEngine::Update()
{
	world = XMMatrixTranslation(m_object->pos.x, m_object->pos.y, m_object->pos.z);

	// Get the world, view, and projection matrices
	camera[0]->GetViewMatrix(view);
	//D3DClass::GetInstance().GetWorldMatrix(world);
	D3DClass::GetInstance().GetProjectionMatrix(projection);
	static float r = 0;
	r += (float)XM_PI*-0.005f;
	world = XMMatrixRotationX(r);

	camera[0]->Update();

	m_object->Update();
}

void GameEngine::Render2D()
{
	m_object->Render2D();
}

void GameEngine::Render3D()
{
	m_object->Render3D();
}


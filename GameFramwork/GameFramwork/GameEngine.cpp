#include "GameEngine.h"
#include "GEUtility.h"
#include "WinMain.h"
#include "Scene1.h"

GameEngine::GameEngine()
{
}

GameEngine::GameEngine(const GameEngine&)
{
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
	Camera* defCamera = new Camera;
	if(!defCamera)
		return false;

	result = defCamera->Initialize(XMFLOAT3(0.0f, 0.0f, -100.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f),
		ToRadian(60.0f), static_cast<float>(WindowMain::GetInstance().GetScreenWidth()) / static_cast<float>(WindowMain::GetInstance().GetScreenHeight()), 1.0f, 500.0f);
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize default camera."), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return false;
	}
	m_cameras.cameras.insert(pair<string, Camera*>("Default", defCamera));
	m_cameras.curCamera = defCamera;

	// Create and initialize the light and light shader
	lightShader = new LightShader;
	if(!lightShader)
		return false;

	result = lightShader->Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize the light shader object."), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
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
		WindowMain::GetInstance().runFlag = false;
		return false;
	}

	// Initialize matrix of world, projection, view and ortho
	world = XMMatrixIdentity();
	view = XMMatrixIdentity();
	projection = XMMatrixIdentity();

	// Regist first scene
	Scene1* scene1 = new Scene1;
	if(!scene1)
		return false;
	result = scene1->Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize scene1"), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return false;
	}
	m_scenes.scenes.insert(pair<string, Scene*>("scene1", scene1));
	m_scenes.curScene = scene1;
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
		if(model.second)
		{
			model.second->Shutdown();
			delete model.second;
			model.second = nullptr;
		}

	}
	modelList.clear();

	// Release image list
	for(auto image : imageList)
	{
		if(image.second)
		{
			image.second->ShutDown();
			delete image.second;
			image.second = nullptr;
		}
	}
	imageList.clear();
	
	// Release image2D list
	for(auto image : image2DList)
	{
		if(image.second)
		{
			image.second->Shutdown();
			delete image.second;
			image.second = nullptr;
		}
	}
	image2DList.clear();

	// Release object list
	for(auto obj : objList)
	{
		if(obj.second)
		{
			obj.second->Shutdown();
			delete obj.second;
			obj.second = nullptr;
		}
	}
	objList.clear();

	// Release cameras
	for(auto camera : m_cameras.cameras)
	{
		if(camera.second)
		{
			camera.second->Shutdown();
			delete camera.second;
			camera.second = nullptr;
		}
		
	}
	m_cameras.cameras.clear();
	m_cameras.curCamera = nullptr;

	// Release scenes
	for(auto scene : m_scenes.scenes)
	{
		if(scene.second)
		{
			scene.second->Shutdown();
			delete scene.second;
			scene.second = nullptr;
		}
	}
	m_scenes.scenes.clear();
	m_scenes.curScene = nullptr;
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
		WindowMain::GetInstance().runFlag = false;
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
		WindowMain::GetInstance().runFlag = false;
		return false;
	}
	imageList.insert(pair<string, Image*>(name, image));
	return true;
}

bool GameEngine::CreateImage2D(string name, TCHAR * path)
{
	bool result;

	Image2D* image = new Image2D;
	result = image->Initialize(path);
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not initialize the image2D object."), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return false;
	}
	image2DList.insert(pair<string, Image2D*>(name, image));
	return true;
}

void GameEngine::CreateText(char * text, TCHAR * font, float size, Color color, Rect box)
{
	Font::GetInstance().CreateText(text, font, size, color, box);
}

Camera * GameEngine::CreateCamera(string name)
{
	Camera* c = new Camera;
	if (c)
	{
		// Set default parameters of camera.
		c->Initialize(XMFLOAT3(5.0f, 0.0f, -10.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f),
			ToRadian(60.0f), static_cast<float>(WindowMain::GetInstance().GetScreenWidth()) / static_cast<float>(WindowMain::GetInstance().GetScreenHeight()), 1.0f, 500.0f);
	}
	m_cameras.cameras.insert(pair<string, Camera*>(name, c));
	return c;
}

void GameEngine::DrawModel(string name)
{
	auto model = modelList.find(name);
	// Update the maxtrix of view and projection
	m_cameras.curCamera->GetViewMatrix(view);
	m_cameras.curCamera->GetProjectionMatrix(projection);
	// Render mesh
	// Render the model using the light shader
	for(auto mesh : model->second->m_model)
	{
		mesh->RenderBuffers();
		lightShader->Render(mesh, world, view, projection, model->second->GetTexture(), light->GetColor(), light->GetDirection(), m_cameras.curCamera->GetViewDirection(), model->second->m_materials.find(mesh->m_usemtl)->second);
	}
}

void GameEngine::DrawImage(string name)
{
	auto image = imageList.find(name);
	// Render image
	// Get view matrix
	if(image!=imageList.end())
	{
		m_cameras.curCamera->GetViewMatrix(view);

		// Get projection matrix(ortho matrix)
		m_cameras.curCamera->GetOrthoMatrix(projection);
	
		// Turn off depth buffer
		D3DClass::GetInstance().m_effect->DepthBufferOff();
		// Render image
		image->second->Render(100, 100);
	
		// Render image with shader
		texShader->Render(image->second->GetIndexCount(), world, view, projection, image->second->GetTexture());
	
		// Turn on depth buffer
		D3DClass::GetInstance().m_effect->DepthBufferOn();
	}
}

void GameEngine::DrawImage2D(string name, Rect dest, Rect src, float opacity)
{
	auto image = image2DList.find(name);
	if(image != image2DList.end())
	{
		image->second->DrawImage(dest, src, opacity);
	}

}

void GameEngine::DestroyModel(string name)
{
	auto model = modelList.find(name);
	if(model != modelList.end())
	{
		model->second->Shutdown();
		delete model->second;
		model->second = nullptr;
		modelList.erase(name);
	}
}

void GameEngine::DestroyImage(string name)
{
	auto image = imageList.find(name);
	if(image != imageList.end())
	{
		image->second->ShutDown();
		delete image->second;
		image->second = nullptr;
		imageList.erase(name);
	}
}

void GameEngine::DestroyImage2D(string name)
{
	auto image = image2DList.find(name);
	if(image != image2DList.end())
	{
		image->second->Shutdown();
		delete image->second;
		image->second = nullptr;
		image2DList.erase(name);
	}
}

void GameEngine::DestroyCamera(string name)
{
	auto camera = m_cameras.cameras.find(name);
	if(camera != m_cameras.cameras.end())
	{
		if(m_cameras.curCamera == camera->second)
		{
			m_cameras.curCamera = m_cameras.cameras.find("Default")->second;
		}
		camera->second->Shutdown();
		delete camera->second;
		camera->second = nullptr;
		m_cameras.cameras.erase(name);
	}

}

void GameEngine::ChangeCamera(string name)
{
	if (m_cameras.cameras.find(name) != m_cameras.cameras.end())
		m_cameras.curCamera = m_cameras.cameras.find(name)->second;
	else
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not find camera"), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return;
	}
}

Camera * GameEngine::FindCamera(string name)
{
	auto camera = m_cameras.cameras.find(name);
	if(camera != m_cameras.cameras.end())
		return camera->second;
	else
		return nullptr;
}

void GameEngine::PushBackObject(string name, Object * object)
{
	// If the object is already in the list than do nothing
	auto obj = objList.find(name);
	if(obj != objList.end())
		return;
	objList.insert(pair<string, Object*>(name, object));
}

void GameEngine::DestroyObject(string name)
{
	auto object = objList.find(name);
	if(object!=objList.end())
	{
		object->second->Shutdown();
		delete object->second;
		object->second = nullptr;
		objList.erase(name);
	}
}

void GameEngine::GetModelSize(const string name, float& width, float& height, float& depth)
{
	auto model = modelList.find(name);
	if (model != modelList.end())
	{
		width = model->second->m_width;
		height = model->second->m_height;
		depth = model->second->m_depth;
	}
}

void GameEngine::QuitApplication()
{
	WindowMain::GetInstance().runFlag = false;
}

void GameEngine::Update()
{
	// Update camera
	m_cameras.curCamera->Update();

	// update current scene.
	if(m_scenes.curScene)
		m_scenes.curScene->Update();
	else
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not find current scene."), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return;
	}

}

void GameEngine::Render2D()
{
	// Render 2D object which in current scene.
	if(m_scenes.curScene)
		m_scenes.curScene->Render2D();
	else
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not find current scene."), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return;
	}
}

void GameEngine::Render3D()
{
	// Render 3D object which in current scene.
	if(m_scenes.curScene)
		m_scenes.curScene->Render3D();
	else
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not find current scene."), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return;
	}
}

void GameEngine::PushBackScene(string name, Scene * scene)
{
	// If the scene is already in the list than do nothing
	auto s = m_scenes.scenes.find(name);
	if(s != m_scenes.scenes.end())
		return;
	// Push back new scene.
	m_scenes.scenes.insert(pair<string, Scene*>(name, scene));
}

void GameEngine::ChangeScene(string name)
{
	m_scenes.curScene->Shutdown();
	if(m_scenes.scenes.find(name)!=m_scenes.scenes.end())
		m_scenes.curScene = m_scenes.scenes.find(name)->second;
	else
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not find scene"), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return;
	}
	bool result = m_scenes.curScene->Initialize();
	if(!result)
	{
		MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Could not intialize scene"), _T("Error"), MB_OK);
		WindowMain::GetInstance().runFlag = false;
		return;
	}
}


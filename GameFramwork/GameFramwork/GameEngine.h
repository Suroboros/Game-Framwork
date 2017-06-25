#pragma once
/////////////////////////////////////
// GameEngine.cpp
// Provide interface to user
/////////////////////////////////////

#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

// Include
#include <string>
#include "Mesh.h"
#include "InputSystem.h"
#include "Image.h"
#include "Image2D.h"
#include "Object.h"
#include "Font.h"
#include "Camera.h"
#include "Light.h"
#include "LightShader.h"
#include "TextureShader.h"
#include "Scene.h"

// GameEngine class
class GameEngine
{
	struct Scenes
	{
		Scene* curScene;
		map<string, Scene*> scenes;
	};

	struct Cameras
	{
		Camera* curCamera;
		map<string, Camera*> cameras;
	};

public:
	bool Initialize();
	void Shutdown();

	static GameEngine& GetInstance();

	void Update();
	void Render2D();
	void Render3D();

	// Functions of Scene
	void PushBackScene(string name, Scene* scene);
	void ChangeScene(string name);

	// Create
	bool CreateModel(string name, TCHAR* modelPath, TCHAR* texPath = nullptr);
	bool CreateImage(string name, TCHAR* path);
	bool CreateImage2D(string name, TCHAR* path);
	void CreateText(char* text,TCHAR* font, float size,Color color, Rect box);
	Camera* CreateCamera(string name);
	// Draw
	void DrawModel(string name);
	void DrawImage(string name);
	void DrawImage2D(string name, Rect dest, Rect src, float opacity = 1.0f);
	// Destroy
	void DestroyModel(string name);
	void DestroyImage(string name);
	void DestroyImage2D(string name);
	void DestroyCamera(string name);

	// Change camera
	void ChangeCamera(string name);
	// Find camera
	Camera* FindCamera(string name);

	// Push back object into list.
	void PushBackObject(string name, Object* object);
	// Find object instance by the name of object.
	template<class T> 
	T* FindObject(string name);
	// Destroy object
	void DestroyObject(string name);

	// Get the size of model
	void GetModelSize(const string name, float& width, float& height, float& depth);

	// Quit application
	void QuitApplication();
	
	// Matrix of world, view, projection, ortho
	XMMATRIX world, view, projection;
private:
	GameEngine();
	GameEngine(const GameEngine&);
	~GameEngine();
	GameEngine& operator=(const GameEngine&);

	

	TextureShader* texShader;
	LightShader* lightShader;

	Light* light;
	Cameras m_cameras;
	Scenes m_scenes;
	map<string, Model*> modelList;
	map<string, Image*> imageList;
	map<string, Image2D*> image2DList;
	map<string, Object*> objList;
	
};



template<class T>
inline T * GameEngine::FindObject(string name)
{
	auto object = objList.find(name);
	if(object != objList.end())
		return dynamic_cast<T*>(object->second);
	else
		return nullptr;
}
#endif // !_GAME_ENGINE_H_
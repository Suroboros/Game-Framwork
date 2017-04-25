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
#include "GEObject.h"
#include "Object.h"
#include "Font.h"
#include "Camera.h"
#include "Light.h"
#include "LightShader.h"
#include "TextureShader.h"

// GameEngine class
class GameEngine
{
public:
	bool Initialize();
	void Shutdown();

	static GameEngine& GetInstance();
	
	bool CreateModel(string name, TCHAR* modelPath, TCHAR* texPath = nullptr);
	bool CreateImage(string name, TCHAR* path);
	void CreateText(TCHAR* text,TCHAR* font, float size,Color color, Rect box);
	
	void DrawModel(string name);
	void DrawImage(string name);

	void DrawCircle(float x, float y, float r);

	
	void Update();
	void Render2D();
	void Render3D();

private:
	GameEngine();
	GameEngine(GameEngine&);
	~GameEngine();
	GameEngine& operator=(const GameEngine&);

	XMMATRIX world, view, projection;

	TextureShader* texShader;
	LightShader* lightShader;

	Camera::spCamera camera[4]; //camera
	Light* light;
	Object* m_object;
	map<string, Model*> modelList;
	map<string, Image*> imageList;
	map<string, Font*> textList;
	
};

#endif // !_GAME_ENGINE_H_

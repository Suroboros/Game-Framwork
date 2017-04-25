#include "Object.h"
#include "GameEngine.h"
#include <AtlConv.h>
Object::Object()
{
	pos = Point{ 0.0f, 0.0f, 0.0f };
	speedX = 0.0f;
	speedY = 0.0f;
}

Object::Object(const Object &)
{
}

Object::~Object()
{
}

bool Object::Initialize()
{
	bool result;
	result = GameEngine::GetInstance().CreateModel("p1", _T("./cube.obj"),_T("./data/image/wood.png"));
	//result = GameEngine::GetInstance().CreateModel("p1", _T("./kuma.obj"));
	if(!result)
		return false;
	

//	result = GameEngine::GetInstance().CreateImage("p1", _T("tex.bmp"));
//	if(!result)
//		return false;

	return true;
}

void Object::Shutdown()
{
}

void Object::Render2D()
{
	int x, y;
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
	
	
	GameEngine::GetInstance().CreateText(temp, _T("Arial"), 50, Color{ 0.0,0.0,1.0,1.0 }, Rect{ float(y), float(x),600,800 });
	
	
}

void Object::Render3D()
{
	GameEngine::GetInstance().DrawModel("p1");
	//GameEngine::GetInstance().DrawImage("p1");
}

void Object::Update()
{
	if(InputSystem::GetInstance().IsKeyDown("sys", DIK_A))
		pos.x -= 5;
	if(InputSystem::GetInstance().IsKeyDown("sys", DIK_D))
		pos.x += 5;
}

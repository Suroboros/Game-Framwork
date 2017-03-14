/////////////////////////////////////
// GraphicSystem.cpp
// Graphic system.
/////////////////////////////////////

// Inludes
#include "GraphicSystem.h"

GraphicSystem::GraphicSystem()
{
}

GraphicSystem::GraphicSystem(const GraphicSystem &)
{
}

GraphicSystem::~GraphicSystem()
{
}

bool GraphicSystem::Initialize(HWND hwnd, int screenWidtH, int screenHight)
{
	return false;
}

void GraphicSystem::Shutdown()
{
}

bool GraphicSystem::Frame()
{
	return true;
}

bool GraphicSystem::Render()
{
	return false;
}

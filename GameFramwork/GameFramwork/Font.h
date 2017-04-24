#pragma once
/////////////////////////////////////
// Font.h
// Font engine.
/////////////////////////////////////

#ifndef _FONT_H_
#define _FONT_H_

// Link 

#pragma comment(lib, "dwrite.lib")

// Include
#include<dwrite.h>
#include<tchar.h>

// Font class
class Font
{
public :
	bool Initialize();
	void Shutdown();

	void CreateText(TCHAR* text, TCHAR* font, FLOAT size, float r, float g, float b, float a);

	// Get the instance of D3DClass(singleton) 
	//static D2DClass& GetInstance();
private:
	
};

#endif // !_FONT_H_
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
#include "GEUtility.h"

// Font class
class Font
{
public :
	bool Initialize();
	void Shutdown();

	static Font& GetInstance();

	void CreateText(TCHAR* text, TCHAR* font, FLOAT size, Color color, Rect box);

	
	
private:
	
};

#endif // !_FONT_H_
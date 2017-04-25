#pragma once
/////////////////////////////////////
// DWClass.h
// Direct write class.
/////////////////////////////////////

#ifndef _DWCLASS_H_
#define _DWCLASS_H_

// Link 

#pragma comment(lib, "dwrite.lib")

// Include
#include<dwrite.h>
#include<tchar.h>

// Font class
class DWClass
{
public:
	bool Initialize();
	void Shutdown();

	static DWClass& GetInstance();

	IDWriteTextFormat* CreateTextFormat(TCHAR* font, FLOAT size);

	// Get the instance of D3DClass(singleton) 
	//static D2DClass& GetInstance();
private:
	DWClass();
	DWClass(const DWClass&);
	~DWClass();
	DWClass& operator=(const DWClass&);

	IDWriteFactory* m_factory;

	IDWriteTextFormat* m_textFormat;
};


#endif // !_DWCLASS_H_

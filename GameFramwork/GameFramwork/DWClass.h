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

	// Get the instance of DWClass(singleton) 
	static DWClass& GetInstance();

	IDWriteTextFormat* CreateTextFormat(TCHAR* font, FLOAT size);

private:
	DWClass();
	DWClass(const DWClass&);
	~DWClass();
	DWClass& operator=(const DWClass&);

	IDWriteFactory* m_factory;

	IDWriteTextFormat* m_textFormat;
};


#endif // !_DWCLASS_H_

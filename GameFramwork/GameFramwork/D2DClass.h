#pragma once
/////////////////////////////////////
// D2DClass.h
// DirectX 2D control system.
/////////////////////////////////////

#ifndef _D2DCLASS_H_
#define _D2DCLASS_H_

// Link 

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")

// Include
#include<d2d1_2.h>
#include<d2d1_2helper.h>
#include"Font.h"

// Direct 2d class
class D2DClass
{
public:
	bool Initialize();
	void Shutdown();

	// Get the instance of D3DClass(singleton) 
	static D2DClass& GetInstance();

	void BeginDraw();
	void EndDraw();
	ID2D1RenderTarget* GetRenderTarget();
	ID2D1DeviceContext* GetDeviceContext();

	ID2D1SolidColorBrush* CreateBrush(float r, float g, float b, float a);

private:
	D2DClass();
	D2DClass(const D2DClass&);
	~D2DClass();
	D2DClass& operator=(const D2DClass&);

	bool m_vsyncEnabled;

	ID2D1Factory1* m_factory;
	ID2D1Device* m_device;
	ID2D1DeviceContext* m_deviceContext;
	IDXGIDevice* m_dxgiDevice;
	IDXGISwapChain* m_swapChain;
	ID2D1RenderTarget* m_renderTarget;

	ID2D1SolidColorBrush* m_solidColorBrush;
	ID2D1Bitmap1* m_bitmap;

	Font* font;
	

};

#endif // !_D2DCLASS_H_
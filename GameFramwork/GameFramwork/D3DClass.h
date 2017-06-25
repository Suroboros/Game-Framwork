#pragma once
/////////////////////////////////////
// D3DClass.h
// DirectX control system.
/////////////////////////////////////

#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

// Link 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Include
#include <d3d11_2.h>
#include <DirectXMath.h>
#include "Effect.h"
using namespace DirectX;

// DirectX class
class D3DClass
{
public:
	bool Initialize(bool vsync, bool fullCreen, float screenDepth, float screenNear);
	void Shutdown();

	// Get the instance of D3DClass(singleton) 
	static D3DClass& GetInstance();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	// Resize if the window size changed
	bool Resize();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();

	Effect* m_effect;
	ID3D11RenderTargetView* m_renderTargetView;
private:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();
	D3DClass& operator=(const D3DClass&);

	bool m_vsyncEnabled;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	
	D3D_FEATURE_LEVEL m_featureLevelSupported;
	ID3D11Texture2D* m_depthBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterizerState;
	
};
//static int acv = 0;
#endif 

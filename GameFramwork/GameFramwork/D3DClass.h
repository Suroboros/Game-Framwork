#pragma once
/////////////////////////////////////
// D3DClass.cpp
// DirectX control system.
/////////////////////////////////////

#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


// Link 

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Include
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;

// DirectX class
class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullCreen, float screenDepth, float screenNear);
	void Shutdown();

	void BeginScene(float red, float green, float blue, float alpha);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetWorldMatrix(XMMATRIX& world);
	void GetProjectionMatrix(XMMATRIX& projection);
	void GetOrthoMatrix(XMMATRIX& ortho);

private:
	bool vsyncEnabled;

	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	ID3D11RenderTargetView* renderTargetView;
	D3D_FEATURE_LEVEL featureLevelSupported;
	ID3D11Texture2D* depthBuffer;
	//ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11RasterizerState* rasterizerState;
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT4X4 orthoMatrix;
};

#endif 

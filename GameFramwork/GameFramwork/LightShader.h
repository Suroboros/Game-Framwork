#pragma once
/////////////////////////////////////
// LightShader.h
// Maintain the shader of light
/////////////////////////////////////

#ifndef _LIGHT_SHADER_H_
#define _LIGHT_SHADER_H_

// Include
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <tchar.h>

using namespace std;
using namespace DirectX;

// Light shader class
class LightShader
{
	struct MaxtrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct LightBufferType
	{
		XMFLOAT4 color;
		XMFLOAT3 direction;
		bool useTex;
	};

public:
	LightShader();
	LightShader(const LightShader& lightShader);
	~LightShader();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();
	bool Render(ID3D11DeviceContext* deviceContext, const int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, const XMFLOAT4 color, const XMFLOAT3 direction);

private:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11SamplerState* samplerState;
	ID3D11Buffer* matrixBuffer;
	ID3D11Buffer* lightBuffer;

	bool InitializeShader(ID3D11Device* device, HWND hwnd, TCHAR* vsPath, TCHAR* psPath);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3DBlob* errorMsg, HWND hwnd, TCHAR* shaderPath);

	bool SetShaderParameter(ID3D11DeviceContext* deviceContex, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& project, ID3D11ShaderResourceView* shaderRsrcView, const XMFLOAT4 color, const XMFLOAT3 direction);
	void RenderShader(ID3D11DeviceContext* deviceContext, const int indexCount);
};

#endif // !_LIGHT_SHADER_H_
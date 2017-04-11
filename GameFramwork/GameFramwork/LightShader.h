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
#include "Mesh.h"

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
		XMFLOAT3 viewDir;
		float padding;
	};

public:
	LightShader();
	LightShader(const LightShader& lightShader);
	~LightShader();

	bool Initialize();
	void Shutdown();
	bool Render(const Mesh* mesh, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture, const XMFLOAT4 color, const XMFLOAT3 direction, const XMFLOAT3 viewDirection);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_samplerState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_materialBuffer;

	bool InitializeShader(TCHAR* vsPath, TCHAR* psPath);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3DBlob* errorMsg, TCHAR* shaderPath);

	bool SetShaderParameter(const Mesh* mesh, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& project, ID3D11ShaderResourceView* shaderRsrcView, const XMFLOAT4 color, const XMFLOAT3 direction, const XMFLOAT3 viewDirection);
	void RenderShader(const int indexCount);
};

#endif // !_LIGHT_SHADER_H_
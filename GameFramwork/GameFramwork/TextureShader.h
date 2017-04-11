#pragma once
/////////////////////////////////////
// TextureShader.h
// Maintain the shader of texture
/////////////////////////////////////

#ifndef _TEXTURE_SHADER_H_
#define _TEXTURE_SHADER_H_

// Include
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include <tchar.h>

using namespace std;
using namespace DirectX;

// Texture shader class
class TextureShader
{
	struct MaxtrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	TextureShader();
	TextureShader(const TextureShader& textureShader);
	~TextureShader();

	bool Initialize();
	void Shutdown();
	bool Render(int indexCount, const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& projection, ID3D11ShaderResourceView* texture);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11SamplerState* m_samplerState;

	bool InitializeShader(TCHAR* vsPath, TCHAR* psPath);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3DBlob* errorMsg, TCHAR* shaderPath);

	bool SetShaderParameter(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& project, ID3D11ShaderResourceView* shaderRsrcView);
	void RenderShader(const int indexCount);
};

#endif // !_TEXTURE_SHADER_H_

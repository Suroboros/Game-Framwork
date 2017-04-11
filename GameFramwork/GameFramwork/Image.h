#pragma once
/////////////////////////////////////
// Image.h
// For presenting 2D image
/////////////////////////////////////

#ifndef _IMAGE_H_
#define _IMAGE_H_


// Include
#include <d3d11.h>
#include <DirectXMath.h>
#include "Texture.h"
#include "GEUtility.h"
using namespace DirectX;

// Image class
class Image
{
	struct ImageVertexType
	{
		XMFLOAT3 pos;
		XMFLOAT2 tex;
	};

public:
	Image();
	Image(const Image& image);
	~Image();

	bool Initialize(TCHAR* imagePath);
	void ShutDown();
	bool Render(int posX, int posY);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
	int m_vertexCount, m_indexCount;
	Texture* m_texture;

	int m_screenWidth, m_screenHeight;
	int m_imageWidth, m_imageHeight;
	int m_prePosX, m_prePosY;

	// Initialize the vertex and index buffers.
	bool InitializeBuffers();
	// Release the vertex buffer and index buffer.
	void ShutdownBuffer();
	// Update the vertex buffer and index buffer
	bool UpdateBuffer(int posX, int posY);
	// Setup the vertex buffer and index buffer.
	void RenderBuffers();

	bool LoadTexture(TCHAR* texPath);
	void ReleaseTexture();
};


#endif // !_IMAGE_H_

#pragma once
/////////////////////////////////////
// Texture.h
// Loading, unloading, and accessing of a single texture resource
/////////////////////////////////////

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

// Include
#include <d3d11.h>
#include <string>
#include <fstream>
using namespace std;

// Texture class
class Texture
{
public:
	Texture();
	Texture(Texture& texture);
	~Texture();

	bool Initialize(TCHAR* srcPath);
	void ShutDown();

	bool LoadImageFromFile(TCHAR* srcPath);

	// Get the texture size
	UINT GetWidth();
	UINT GetHeight();

	ID3D11ShaderResourceView* GetTexture();

private:
	// Image infor
	unsigned char* m_imgBuffer;
	UINT m_width, m_height;
	UINT m_stride;
	DXGI_FORMAT m_ImageFormat;

	ID3D11Texture2D* m_texture;
	ID3D11ShaderResourceView* m_textureView;
	//unsigned char* imageData;
};

#endif // !_TEXTURE_H_
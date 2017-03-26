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

	bool Initialize(ID3D11Device* device,ID3D11DeviceContext* deviceContext, TCHAR* srcPath);
	void ShutDown();

	bool LoadImageFromFile(TCHAR* srcPath);

	ID3D11ShaderResourceView* GetTexture();

private:
	// Image infor
	unsigned char* imgBuffer;
	UINT width, height;
	UINT stride;

	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* textureView;
	//unsigned char* imageData;
};

#endif // !_TEXTURE_H_
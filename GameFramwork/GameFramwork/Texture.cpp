#include "Texture.h"


#pragma warning(push)
#pragma warning(disable : 4005)
#include <wincodec.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)

#include <assert.h>
#include <dxgiformat.h>
using namespace std;
#if (_WIN32_WINNT >= 0x0602 /*Windows 8,10 WIC*/) && !defined(DXGI_1_2_FORMATS)
#define DXGI_1_2_FORMATS
#endif
// Format translate struct
struct WICFormatTranslate
{
	GUID                pix;
	DXGI_FORMAT         dxgi;
};

// Format translate list
WICFormatTranslate transList[] =
{
	{ GUID_WICPixelFormat128bppRGBAFloat,   DXGI_FORMAT_R32G32B32A32_FLOAT },
	{ GUID_WICPixelFormat64bppRGBAHalf,      DXGI_FORMAT_R16G16B16A16_FLOAT },
	{ GUID_WICPixelFormat64bppRGBA,          DXGI_FORMAT_R16G16B16A16_UNORM },
	{ GUID_WICPixelFormat32bppRGBA,          DXGI_FORMAT_R8G8B8A8_UNORM },
	{ GUID_WICPixelFormat32bppBGRA,          DXGI_FORMAT_B8G8R8A8_UNORM },//(DXGI 1.1)
	{ GUID_WICPixelFormat32bppBGR,           DXGI_FORMAT_B8G8R8X8_UNORM },//(DXGI 1.1)
	{ GUID_WICPixelFormat32bppRGBA1010102XR, DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM },//(DXGI 1.1)
	{ GUID_WICPixelFormat32bppRGBA1010102,   DXGI_FORMAT_R10G10B10A2_UNORM },
	{ GUID_WICPixelFormat32bppRGBE,          DXGI_FORMAT_R9G9B9E5_SHAREDEXP },
#ifdef DXGI_1_2_FORMATS
	{ GUID_WICPixelFormat16bppBGRA5551,      DXGI_FORMAT_B5G5R5A1_UNORM },//(DXGI 1.2)
	{ GUID_WICPixelFormat16bppBGR565,        DXGI_FORMAT_B5G6R5_UNORM },//(DXGI 1.2)
#endif // DXGI_1_2_FORMATS
	{ GUID_WICPixelFormat32bppGrayFloat,     DXGI_FORMAT_R32_FLOAT },
	{ GUID_WICPixelFormat16bppGrayHalf,      DXGI_FORMAT_R16_FLOAT },
	{ GUID_WICPixelFormat16bppGray,          DXGI_FORMAT_R16_UNORM },
	{ GUID_WICPixelFormat8bppGray,           DXGI_FORMAT_R8_UNORM },
	{ GUID_WICPixelFormat8bppAlpha,          DXGI_FORMAT_A8_UNORM },
#if (_WIN32_WINNT >= 0x0602 /*Windows 8,10 WIC*/)
	{ GUID_WICPixelFormat96bppRGBFloat,	DXGI_FORMAT_R32G32B32_FLOAT }
#endif
};

Texture::Texture()
{
	texture = nullptr;
	textureView = nullptr;
}

Texture::Texture(Texture & texture)
{
}

Texture::~Texture()
{
}

bool Texture::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, string srcPath)
{
	bool result;
	HRESULT hr;

	// Initialize COM
	CoInitialize(NULL);

	// Create the imageing factory interface
	IWICImagingFactory* imgFactory = nullptr;
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (LPVOID*)&imgFactory);
	if(FAILED(hr))
		return false;

	IWICBitmapDecoder* imgDecoder = nullptr;
	hr = imgFactory->CreateDecoderFromFilename(L"./1.bmp", NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &imgDecoder);
	if(FAILED(hr))
		return false;

	// Get frame from decoder
	IWICBitmapFrameDecode* imgFrame = nullptr;
	hr = imgDecoder->GetFrame(0, &imgFrame);
	if(FAILED(hr))
		return false;

	// Get the width and height of image
	UINT width, height;
	hr = imgFrame->GetSize(&width, &height);
	if(FAILED(hr))
		return false;
	assert(width > 0 && height > 0);

	// Get the pixel format of the image.
	WICPixelFormatGUID pixFormat;
	hr = imgFrame->GetPixelFormat(&pixFormat);
	if(FAILED(hr))
		return false;

	// Convert pixel format to dxgi format
	DXGI_FORMAT dxgiFormat = DXGI_FORMAT_UNKNOWN;
	for(int i = 0; i < _countof(transList); ++i)
	{
		if(memcmp(&transList[i].pix, &pixFormat, sizeof(pixFormat)) == 0)
			dxgiFormat = transList[i].dxgi;
	}

	IWICFormatConverter* fc = nullptr;
	// Change to 32 bits data
	if(dxgiFormat != DXGI_FORMAT_R8G8B8A8_UNORM)
	{
		hr = imgFactory->CreateFormatConverter(&fc);
		if(FAILED(hr))
			return false;

		hr = fc->Initialize(imgFrame, GUID_WICPixelFormat32bppBGRA, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
		if(FAILED(hr))
			return false;
	}

	// Get bpp of the image
	IWICComponentInfo* cinfo = nullptr;
	if(FAILED(imgFactory->CreateComponentInfo(pixFormat, &cinfo)))
		return false;

	WICComponentType type;
	if(FAILED(cinfo->GetComponentType(&type)))
		return 0;

	if(type != WICPixelFormat)
		return 0;

	IWICPixelFormatInfo* pfinfo;
	if(FAILED(cinfo->QueryInterface(__uuidof(IWICPixelFormatInfo), reinterpret_cast<void**>(&pfinfo))))
		return 0;

	UINT bpp;
	if(FAILED(pfinfo->GetBitsPerPixel(&bpp)))
		return 0;

	// Copy image data
	//unsigned char* imgBuffer = new unsigned char[width*height * 4];
	unsigned char* imgBuffer = new unsigned char[width*height * bpp];
	UINT stride = (width * bpp + 7) / 8;
	UINT size = stride*height;
	hr = imgFrame->CopyPixels(0, stride, size, imgBuffer);
	if(FAILED(hr))
		return false;

	// Release resource
	pfinfo->Release();
	pfinfo = nullptr;

	cinfo->Release();
	cinfo = nullptr;

	fc->Release();
	fc = nullptr;

	imgFrame->Release();
	imgFrame = nullptr;

	imgDecoder->Release();
	imgDecoder = nullptr;

	imgFactory->Release();
	imgFactory = nullptr;

	// Setup the description of the texture
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Height = height;
	texDesc.Width = width;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	// Create the empty texture
	hr = device->CreateTexture2D(&texDesc, NULL, &texture);
	if(FAILED(hr))
		return false;

	// Copy the image data into the texture
	deviceContext->UpdateSubresource(texture, 0, NULL, imgBuffer, stride, 0);

	// Setup the shader resource view description
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	// Create the shader resource view for the texture
	hr = device->CreateShaderResourceView(texture, &srvDesc, &textureView);
	if(FAILED(hr))
		return false;

	// Generate mipmaps for this texture
	deviceContext->GenerateMips(textureView);

	// Release image buffer
	delete[] imgBuffer;
	imgBuffer = nullptr;

	return true;
}

void Texture::ShutDown()
{
	// Release the texture view resource
	if(textureView)
	{
		textureView->Release();
		textureView = nullptr;
	}

	// Release the texture;
	if(texture)
	{
		texture->Release();
		texture = nullptr;
	}
}

ID3D11ShaderResourceView * Texture::GetTexture()
{
	return textureView;
}

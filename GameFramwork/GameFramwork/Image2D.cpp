#include "Image2D.h"


#pragma warning(push)
#pragma warning(disable : 4005)
#include <wincodec.h>
#pragma warning(pop)

Image2D::Image2D()
{
	m_Bitmap = nullptr;
}

Image2D::Image2D(Image2D & image)
{
}

Image2D::~Image2D()
{
}

bool Image2D::Initialize(LPCTSTR srcPath)
{
	bool result;
	result = LoadImageFromFile(srcPath);

	return result;
}

void Image2D::Shutdown()
{
	m_Bitmap->Release();
	m_Bitmap = nullptr;
}

bool Image2D::LoadImageFromFile(LPCTSTR srcPath)
{
	HRESULT hr;

	// Initialize COM
	CoInitialize(NULL);

	// Create the imageing factory interface
	IWICImagingFactory* imgFactory = nullptr;
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IWICImagingFactory), (LPVOID*)&imgFactory);
	if(FAILED(hr))
		return false;

	IWICBitmapDecoder* imgDecoder = nullptr;
	hr = imgFactory->CreateDecoderFromFilename(srcPath, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &imgDecoder);
	if(FAILED(hr))
		return false;

	// Get frame from decoder
	IWICBitmapFrameDecode* imgFrame = nullptr;
	hr = imgDecoder->GetFrame(0, &imgFrame);
	if(FAILED(hr))
		return false;

	// Create format converter
	IWICFormatConverter* fc = nullptr;
	hr = imgFactory->CreateFormatConverter(&fc);
	if(FAILED(hr))
		return false;

	// Convert format to 32bppPBGRA
	hr = fc->Initialize(imgFrame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	if(FAILED(hr))
		return false;

	// Create a Direct2D bitmap from the WIC bitmap.
	hr = D2DClass::GetInstance().GetDeviceContext()->CreateBitmapFromWicBitmap(fc, NULL, &m_Bitmap);
	if(FAILED(hr))
		return false;

	// Release resource
	fc->Release();
	fc = nullptr;

	imgFrame->Release();
	imgFrame = nullptr;

	imgDecoder->Release();
	imgDecoder = nullptr;

	imgFactory->Release();
	imgFactory = nullptr;

	return true;
}

void Image2D::DrawImage(Rect destination,Rect source, float opacity)
{
	D2DClass::GetInstance().GetDeviceContext()->DrawBitmap(m_Bitmap, D2D1::RectF(destination.left, destination.top, destination.left + destination.width, destination.top + destination.height), opacity, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, D2D1::RectF(source.left, source.top, source.left + source.width, source.top + source.height));
}

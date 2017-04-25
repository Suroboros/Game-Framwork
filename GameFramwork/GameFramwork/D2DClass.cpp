#include "D2DClass.h"
#include "D3DClass.h"

D2DClass::D2DClass()
{
	m_factory = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_dxgiDevice = nullptr;
	m_swapChain = nullptr;
	m_renderTarget = nullptr;
	m_bitmap = nullptr;

}

D2DClass::D2DClass(const D2DClass&)
{
}

D2DClass::~D2DClass()
{
}

D2DClass & D2DClass::operator=(const D2DClass &)
{
	return *this;
}

bool D2DClass::Initialize()
{
	HRESULT hr;

	// Create d2d factory
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory);
	if(FAILED(hr))
		return false;

	// Get DXGI device from d3dclass
	hr = D3DClass::GetInstance().GetDevice()->QueryInterface<IDXGIDevice>(&m_dxgiDevice);
	if(FAILED(hr))
		return false;
	
	// Create d2d device from DXGI device
	hr = m_factory->CreateDevice(m_dxgiDevice, &m_device);
	if(FAILED(hr))
		return false;

	// Create d2d device context
	hr = m_device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_deviceContext);
	if(FAILED(hr))
		return false;
		
	// Get the back buffer
	IDXGISurface* backBufferPtr;
	hr = D3DClass::GetInstance().GetSwapChain()->GetBuffer(0, IID_PPV_ARGS(&backBufferPtr));
	if(FAILED(hr))
		return false;

	// Set render target properties
	FLOAT dpix, dpiy;
	m_factory->GetDesktopDpi(&dpix, &dpiy);
	D2D1_RENDER_TARGET_PROPERTIES prop;
	ZeroMemory(&prop, sizeof(prop));
	prop.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	prop.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	prop.dpiX = dpix;
	prop.dpiY = dpiy;


	D2D1_BITMAP_PROPERTIES1 bitmapProperties; 
	ZeroMemory(&bitmapProperties, sizeof(bitmapProperties));
	bitmapProperties.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	bitmapProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE);
	bitmapProperties.dpiX = dpix;
	bitmapProperties.dpiY = dpiy;




	// Set render target
	//hr = m_factory->CreateDxgiSurfaceRenderTarget(backBufferPtr, &prop, &m_renderTarget);
	hr = m_deviceContext->CreateBitmapFromDxgiSurface(backBufferPtr, &bitmapProperties, &m_bitmap);
	if(FAILED(hr))
		return false;

	m_deviceContext->SetTarget(m_bitmap);
	
	// Release
	backBufferPtr->Release();
	backBufferPtr = nullptr;


	
	return true;
}

void D2DClass::Shutdown()
{
	if (m_bitmap)
	{
		m_bitmap->Release();
		m_bitmap = nullptr;
	}

	// Release render target
	if(m_renderTarget)
	{
		m_renderTarget->Release();
		m_renderTarget = nullptr;
	}

	// Release device context
	if(m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	// Release device
	if(m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	// Release factory
	if(m_factory)
	{
		m_factory->Release();
		m_factory = nullptr;
	}

	// Set digx device and swapchain pointer to null because they will be released in D3DClass
	if(m_dxgiDevice)
		m_dxgiDevice = nullptr;
	if(m_swapChain)
		m_swapChain = nullptr;

}

D2DClass & D2DClass::GetInstance()
{
	static D2DClass instance;
	return instance;
}

void D2DClass::BeginDraw()
{
	//m_renderTarget->Clear();
	//m_renderTarget->BeginDraw();
	m_deviceContext->BeginDraw();
}

void D2DClass::EndDraw()
{
	//m_renderTarget->EndDraw();
	m_deviceContext->EndDraw();

}

ID2D1RenderTarget * D2DClass::GetRenderTarget()
{
	return m_renderTarget;
}

ID2D1DeviceContext* D2DClass::GetDeviceContext()
{
	return m_deviceContext;
}

ID2D1SolidColorBrush* D2DClass::CreateBrush(float r, float g, float b, float a)
{
	//m_renderTarget->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &m_solidColorBrush);
	m_deviceContext->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &m_solidColorBrush);
	return m_solidColorBrush;
}

void D2DClass::DrawCircle(float x, float y, float r)
{
	m_deviceContext->DrawLine(D2D1::Point2F(x - 10.0f,y), D2D1::Point2F( x + 10.0f,y), CreateBrush(1.0, 0.0, 0.0, 0.0), 10);

}

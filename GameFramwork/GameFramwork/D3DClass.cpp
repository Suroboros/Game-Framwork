/////////////////////////////////////
// D3DClass.cpp
// DirectX control system.
/////////////////////////////////////

#include "D3DClass.h"

D3DClass::D3DClass()
{
	vsyncEnabled = false;

	device = nullptr;
	deviceContext = nullptr;
	swapChain = nullptr;
	renderTargetView = nullptr;
	depthBuffer = nullptr;
	//depthStencilState = nullptr;
	depthStencilView = nullptr;
	rasterizerState = nullptr;
}

D3DClass::D3DClass(const D3DClass &)
{
}

D3DClass::~D3DClass()
{
}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullCreen, float screenDepth, float screenNear)
{
	HRESULT hr;

	vsyncEnabled = vsync;

	// Create a graphics interface factory
	IDXGIFactory* factory = nullptr;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if(FAILED(hr))
		return false;

	// Get the graphics interface(video card)
	IDXGIAdapter* adapter = nullptr;
	if(factory->EnumAdapters(0, &adapter) == DXGI_ERROR_NOT_FOUND)
		return false;

	// Enumerate the primary adapter output(monitor).
	IDXGIOutput* adapterOutput = nullptr;
	if(adapter->EnumOutputs(0, &adapterOutput) == DXGI_ERROR_NOT_FOUND)
		return false;

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output(monitor).
	UINT numModes = 0;
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if(FAILED(hr))
		return false;

	// Create a list to hold all possible modes for this monitor/video card combination and fill the list.
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
		return false;
	hr = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if(FAILED(hr))
		return false;

	// Find matched screen width and height, then store the numerator and denominator of the refresh rate.
	UINT numerator, denominator;
	for(int i = 0; i < static_cast<int>(numModes); ++i)
	{
		if(displayModeList[i].Width == static_cast<UINT>(screenWidth))
		{
			if(displayModeList[i].Height == static_cast<UINT>(screenHeight))
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}
	
	// Release the display mode list
	delete[] displayModeList;
	displayModeList = nullptr;

	// Release the adapter output
	adapterOutput->Release();
	adapterOutput = nullptr;

	// Release the adapter
	adapter->Release();
	adapter = nullptr;

	// Release the factory
	factory->Release();
	factory = nullptr;

	// Setup the feature level
	const D3D_FEATURE_LEVEL featureLevels[] = { 
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1 
	};
	UINT featureLevelsCount = 7;

	// Create swap chain
	DXGI_SWAP_CHAIN_DESC swDesc;
	ZeroMemory(&swDesc, sizeof(swDesc));
	swDesc.BufferCount = 1;
	swDesc.BufferDesc.Width = screenWidth;
	swDesc.BufferDesc.Height = screenHeight;
	swDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if(vsyncEnabled)
	{
		swDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swDesc.BufferDesc.RefreshRate.Numerator = 0;
		swDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swDesc.OutputWindow = hwnd;
	swDesc.SampleDesc.Count = 1;
	swDesc.SampleDesc.Quality = 0;
	swDesc.Windowed = !fullCreen;
	swDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, featureLevelsCount,
		D3D11_SDK_VERSION, &swDesc, &swapChain, &device, &featureLevelSupported, &deviceContext);
	if (FAILED(hr))
		return false;
	
	// Get the back buffer pointer and set it as target view.
	ID3D11Texture2D* backBufferPtr;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(hr))
		return false;
	hr = device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView);
	// Release the back buffer because we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = nullptr;
	if (FAILED(hr))
		return false;
	// Set render target
	deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);
	
	// Create depth buffer
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&depthBufferDesc, NULL, &depthBuffer);
	if (FAILED(hr))
		return false;

	// Create depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	depthStencilViewDesc.Format = depthBufferDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Flags = 0;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(hr))
		return false;

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	// Setup the raster descripthion which will determine how and what polygons will be drawn
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;
	// Create the rasterizer state
	hr = device->CreateRasterizerState(&rasterDesc, &rasterizerState);
	if(FAILED(hr))
		return false;
	// Set the rasterizer state
	deviceContext->RSSetState(rasterizerState);

	// Setup the viewport for rendering
	D3D11_VIEWPORT viewPort[1];
	viewPort[0].Width = static_cast<FLOAT>(screenWidth);
	viewPort[0].Height = static_cast<FLOAT>(screenHeight);
	viewPort[0].TopLeftX = 0.0f;
	viewPort[0].TopLeftY = 0.0f;
	viewPort[0].MinDepth = 0.0f;
	viewPort[0].MaxDepth = 1.0f;
	// Create the viewport
	deviceContext->RSSetViewports(_countof(viewPort), viewPort);

	return true;
}

void D3DClass::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(swapChain)
	{
		swapChain->SetFullscreenState(false, NULL);
	}

	if(deviceContext)
	{
		deviceContext->ClearState();
	}

	if(rasterizerState)
	{
		rasterizerState->Release();
		rasterizerState = nullptr;
	}

	if(depthStencilView)
	{
		depthStencilView->Release();
		depthStencilView = nullptr;
	}

/*	if(depthStencilState)
	{
		depthStencilState->Release();
		depthStencilState = nullptr;
	}
*/
	if(depthBuffer)
	{
		depthBuffer->Release();
		depthBuffer = nullptr;
	}

	if(renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	if(deviceContext)
	{
		deviceContext->Release();
		deviceContext = nullptr;
	}

	if(device)
	{
		device->Release();
		device = nullptr;
	}

	if(swapChain)
	{
		swapChain->Release();
		swapChain = nullptr;
	}

	return;
}

// The beginning of each frame.
void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
	// Setup the color to clear the buffer to
	float color[4] = { red,green,blue,alpha };

	// Clear the back buffer
	deviceContext->ClearRenderTargetView(renderTargetView, color);

	// Clear the depth buffer
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

// The end of each frame.
void D3DClass::EndScene()
{
	// Present the back buffer to the screen since rendering is complete.
	if(vsyncEnabled)
	{
		// Lock to screen refresh rate.
		swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		swapChain->Present(0, 0);
	}

	return;
}

ID3D11Device * D3DClass::GetDevice()
{
	return device;
}

ID3D11DeviceContext * D3DClass::GetDeviceContext()
{
	return deviceContext;
}

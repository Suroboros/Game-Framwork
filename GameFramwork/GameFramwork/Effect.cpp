#include "Effect.h"
#include "D3DClass.h"

Effect::Effect()
{
	depthStencilState2D = nullptr;
	depthStencilState3D = nullptr;
}

Effect::Effect(const Effect & effect)
{
}

Effect::~Effect()
{
}

bool Effect::Initialize()
{
	HRESULT hr;
	
	// Initialize the depth stencil state
	// Setup the description of depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create default 3D rendering
	depthStencilDesc.DepthEnable = true;
	hr = D3DClass::GetInstance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &depthStencilState3D);
	if(FAILED(hr))
		return false;

	// Create default 2D rendering
	depthStencilDesc.DepthEnable = false;
	hr = D3DClass::GetInstance().GetDevice()->CreateDepthStencilState(&depthStencilDesc, &depthStencilState2D);
	if(FAILED(hr))
		return false;


	return true;
}

void Effect::Shutdown()
{
	// Release the default 3D render depth stencil state
	if(depthStencilState3D)
	{
		depthStencilState3D->Release();
		depthStencilState3D = nullptr;
	}

	// Release the default 2D render depth stencil state
	if(depthStencilState2D)
	{
		depthStencilState2D->Release();
		depthStencilState2D = nullptr;
	}
}

void Effect::DepthBufferOn()
{
	D3DClass::GetInstance().GetDeviceContext()->OMSetDepthStencilState(depthStencilState3D, 1);
}

void Effect::DepthBufferOff()
{
	D3DClass::GetInstance().GetDeviceContext()->OMSetDepthStencilState(depthStencilState2D, 1);
}

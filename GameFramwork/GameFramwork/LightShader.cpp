#include "LightShader.h"

LightShader::LightShader()
{
	vertexShader = nullptr;
	pixelShader = nullptr;
	layout = nullptr;
	samplerState = nullptr;
	matrixBuffer = nullptr;
	lightBuffer = nullptr;
}

LightShader::LightShader(const LightShader & lightShader)
{
}

LightShader::~LightShader()
{
}

bool LightShader::Initialize(ID3D11Device * device, HWND hwnd)
{
	bool result;
	// Initialize the vertex and pixel shanders
	result = InitializeShader(device, hwnd, _T("./data/shader/Light.vs.hlsl"), _T("./data/shader/Light.ps.hlsl"));

	return result;
}

void LightShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders
	ShutdownShader();
}

bool LightShader::Render(ID3D11DeviceContext * deviceContext, int indexCount, XMMATRIX & world, XMMATRIX & view, XMMATRIX & projection, ID3D11ShaderResourceView * texture, XMFLOAT4 color, XMFLOAT3 direction)
{
	bool result;

	// Set the shader parameters for rendering
	result = SetShaderParameter(deviceContext, world, view, projection, texture, color, direction);
	if(!result)
		return false;

	// Render shader
	RenderShader(deviceContext, indexCount);

	return true;
}

bool LightShader::InitializeShader(ID3D11Device * device, HWND hwnd, TCHAR * vsPath, TCHAR * psPath)
{
	HRESULT hr;
	
	ID3DBlob* errorMsg = nullptr;

	// Compile the vertex shader
	ID3DBlob* vsBuffer = nullptr;
	hr = D3DCompileFromFile(vsPath, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_DEBUG, 0, &vsBuffer, &errorMsg);
	if(FAILED(hr))
	{
		if(errorMsg)
			OutputShaderErrorMessage(errorMsg, hwnd, vsPath);
		else
			MessageBox(hwnd, vsPath, _T("Missing vertex shader file"), MB_OK);

		return false;
	}

	// Compile the pixel shader
	ID3DBlob* psBuffer = nullptr;
	hr = D3DCompileFromFile(psPath, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_DEBUG, 0, &psBuffer, &errorMsg);
	if(FAILED(hr))
	{
		if(errorMsg)
			OutputShaderErrorMessage(errorMsg, hwnd, psPath);
		else
			MessageBox(hwnd, psPath, _T("Missing pixel shader file"), MB_OK);

		return false;
	}
	
	// Create the vertex and pixel shader from the buffer.
	hr = device->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), NULL, &vertexShader);
	if(FAILED(hr))
		return false;
	hr = device->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), NULL, &pixelShader);
	if(FAILED(hr))
		return false;

	// Create the vertex input layout description
	D3D11_INPUT_ELEMENT_DESC inputLayout[3];
	inputLayout[0].SemanticName = "POSITION";
	inputLayout[0].SemanticIndex = 0;
	inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[0].InputSlot = 0;
	inputLayout[0].AlignedByteOffset = 0;
	inputLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[0].InstanceDataStepRate = 0;

	inputLayout[1].SemanticName = "TEXCOORD";
	inputLayout[1].SemanticIndex = 0;
	inputLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputLayout[1].InputSlot = 0;
	inputLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[1].InstanceDataStepRate = 0;

	inputLayout[2].SemanticName = "NORMAL";
	inputLayout[2].SemanticIndex = 0;
	inputLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputLayout[2].InputSlot = 0;
	inputLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	inputLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	inputLayout[2].InstanceDataStepRate = 0;

	// Create the vertex input layout
	hr = device->CreateInputLayout(inputLayout, _countof(inputLayout), vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &layout);
	if(FAILED(hr))
		return false;

	// Release resource
	if(vsBuffer)
	{
		vsBuffer->Release();
		vsBuffer = nullptr;
	}

	if(psBuffer)
	{
		psBuffer->Release();
		psBuffer = nullptr;
	}

	// Create texture sampler state description
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state
	hr = device->CreateSamplerState(&samplerDesc, &samplerState);
	if(FAILED(hr))
		return false;

	// Setup the description of the dynamic matrix constant buffer
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MaxtrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer
	hr = device->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);
	if(FAILED(hr))
		return false;

	// Setup description of the light dynamic constant buffer
	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;

	// Create the constant buffer
	hr = device->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);
	if(FAILED(hr))
		return false;

	return true;
}

void LightShader::ShutdownShader()
{
	// Release resource
	if(lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = nullptr;
	}

	if(matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}

	if(samplerState)
	{
		samplerState->Release();
		samplerState = nullptr;
	}

	if(layout)
	{
		layout->Release();
		layout = nullptr;
	}

	if(pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}

	if(vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}
}

void LightShader::OutputShaderErrorMessage(ID3DBlob * errorMsg, HWND hwnd, TCHAR * shaderPath)
{
	char* compileErrors;
	ofstream fout;

	// Get the error message text buffer pointer
	compileErrors = (char*)errorMsg->GetBufferPointer();

	// Open output file
	fout.open("shader-error-log.txt");

	// Write error message
	fout << compileErrors << endl;

	// Close output file
	fout.close();

	// Release
	errorMsg->Release();
	errorMsg = nullptr;

	// Show a message box to notify to check the log
	MessageBox(hwnd, _T("Error compiling shader. Please check shader-error-log.txt for more informations."), shaderPath, MB_OK);

}

bool LightShader::SetShaderParameter(ID3D11DeviceContext * deviceContex, XMMATRIX & world, XMMATRIX & view, XMMATRIX & project, ID3D11ShaderResourceView * shaderRsrcView, XMFLOAT4 color, XMFLOAT3 direction)
{
	HRESULT hr;

	// Transpose the matrices
	world = XMMatrixTranspose(world);
	view = XMMatrixTranspose(view);
	project = XMMatrixTranspose(project);

	// Set matrices parameter
	// Lock the constant buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = deviceContex->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr))
		return false;

	// Get data pointer
	MaxtrixBufferType* matrixData;
	matrixData = (MaxtrixBufferType*)mappedResource.pData;

	// Copy the matrices
	matrixData->world = world;
	matrixData->view = view;
	matrixData->projection = project;

	// Unlock the constant buffer
	deviceContex->Unmap(matrixBuffer, 0);

	// Set constant buffer in the vertex shader
	deviceContex->VSSetConstantBuffers(0, 1, &matrixBuffer);

	// Set light parameter
	// Lock the constant buffer
	hr = deviceContex->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr))
		return false;

	// Get data pointer
	LightBufferType* lightData;
	lightData = (LightBufferType*)mappedResource.pData;

	// Copy the matrices
	lightData->color = color;
	lightData->direction = direction;
	if(shaderRsrcView)
	{
		lightData->useTex = true;
		// Set shader texture resource in the pixel shader
		deviceContex->PSSetShaderResources(0, 1, &shaderRsrcView);
	}
	else
		lightData->useTex = false;

	// Unlock the constant buffer
	deviceContex->Unmap(lightBuffer, 0);

	// Set shader texture resource in the pixel shader
	deviceContex->PSSetConstantBuffers(0, 1, &lightBuffer);

	return true;
}

void LightShader::RenderShader(ID3D11DeviceContext * deviceContext, int indexCount)
{
	// Set the vertex input layout
	deviceContext->IASetInputLayout(layout);

	// Set the vertex and pixel shader
	deviceContext->VSSetShader(vertexShader, NULL, 0);
	deviceContext->PSSetShader(pixelShader, NULL, 0);

	// Set the sampler state
	deviceContext->PSSetSamplers(0, 1, &samplerState);

	// Render
	deviceContext->DrawIndexed(indexCount, 0, 0);
}

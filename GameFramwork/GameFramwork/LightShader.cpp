#include "LightShader.h"
#include "D3DClass.h"
#include "WinMain.h"

LightShader::LightShader()
{
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_samplerState = nullptr;
	m_matrixBuffer = nullptr;
	m_lightBuffer = nullptr;
}

LightShader::LightShader(const LightShader & lightShader)
{
}

LightShader::~LightShader()
{
}

bool LightShader::Initialize()
{
	bool result;
	// Initialize the vertex and pixel shanders
	result = InitializeShader(_T("./data/shader/Light.vs.hlsl"), _T("./data/shader/Light.ps.hlsl"));

	return result;
}

void LightShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders
	ShutdownShader();
}

bool LightShader::Render(const Mesh* mesh, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & projection, ID3D11ShaderResourceView * texture, const XMFLOAT4 color, const XMFLOAT3 direction, const XMFLOAT3 viewDirection, const MaterialType material)
{
	bool result;

	// Set the shader parameters for rendering
	result = SetShaderParameter(mesh, world, view, projection, texture, color, direction, viewDirection, material);
	if(!result)
		return false;

	// Render shader
	RenderShader(mesh->m_indices.size());

	return true;
}

bool LightShader::InitializeShader(TCHAR * vsPath, TCHAR * psPath)
{
	HRESULT hr;
	
	ID3DBlob* errorMsg = nullptr;

	// Compile the vertex shader
	ID3DBlob* vsBuffer = nullptr;
	hr = D3DCompileFromFile(vsPath, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_DEBUG, 0, &vsBuffer, &errorMsg);
	if(FAILED(hr))
	{
		if(errorMsg)
			OutputShaderErrorMessage(errorMsg, vsPath);
		else
			MessageBox(WindowMain::GetInstance().GetHwnd(), vsPath, _T("Missing vertex shader file"), MB_OK);

		return false;
	}

	// Compile the pixel shader
	ID3DBlob* psBuffer = nullptr;
	hr = D3DCompileFromFile(psPath, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_PACK_MATRIX_COLUMN_MAJOR | D3D10_SHADER_DEBUG, 0, &psBuffer, &errorMsg);
	if(FAILED(hr))
	{
		if(errorMsg)
			OutputShaderErrorMessage(errorMsg, psPath);
		else
			MessageBox(WindowMain::GetInstance().GetHwnd(), psPath, _T("Missing pixel shader file"), MB_OK);

		return false;
	}
	
	// Create the vertex and pixel shader from the buffer.
	hr = D3DClass::GetInstance().GetDevice()->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if(FAILED(hr))
		return false;
	hr = D3DClass::GetInstance().GetDevice()->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), NULL, &m_pixelShader);
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
	hr = D3DClass::GetInstance().GetDevice()->CreateInputLayout(inputLayout, _countof(inputLayout), vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), &m_layout);
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
	hr = D3DClass::GetInstance().GetDevice()->CreateSamplerState(&samplerDesc, &m_samplerState);
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
	hr = D3DClass::GetInstance().GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
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
	hr = D3DClass::GetInstance().GetDevice()->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
	if(FAILED(hr))
		return false;

	// Setup description of the meterial constant buffer
	D3D11_BUFFER_DESC materialBufferDesc;
	materialBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	materialBufferDesc.ByteWidth = sizeof(MaterialType);
	materialBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	materialBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	materialBufferDesc.MiscFlags = 0;
	materialBufferDesc.StructureByteStride = 0;

	// Create the constant buffer
	hr = D3DClass::GetInstance().GetDevice()->CreateBuffer(&materialBufferDesc, NULL, &m_materialBuffer);
	if(FAILED(hr))
		return false;

	return true;
}

void LightShader::ShutdownShader()
{
	// Release resource
	if(m_lightBuffer)
	{
		m_lightBuffer->Release();
		m_lightBuffer = nullptr;
	}

	if(m_matrixBuffer)
	{
		m_matrixBuffer->Release();
		m_matrixBuffer = nullptr;
	}

	if(m_materialBuffer)
	{
		m_materialBuffer->Release();
		m_materialBuffer = nullptr;
	}

	if(m_samplerState)
	{
		m_samplerState->Release();
		m_samplerState = nullptr;
	}

	if(m_layout)
	{
		m_layout->Release();
		m_layout = nullptr;
	}

	if(m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	if(m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}
}

void LightShader::OutputShaderErrorMessage(ID3DBlob * errorMsg, TCHAR * shaderPath)
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
	MessageBox(WindowMain::GetInstance().GetHwnd(), _T("Error compiling shader. Please check shader-error-log.txt for more informations."), shaderPath, MB_OK);

}

bool LightShader::SetShaderParameter(const Mesh* mesh, const XMMATRIX & world, const XMMATRIX & view, const XMMATRIX & project, ID3D11ShaderResourceView * shaderRsrcView, const XMFLOAT4 color, const XMFLOAT3 direction, const XMFLOAT3 viewDirection, const MaterialType material)
{
	HRESULT hr;

	// Transpose the matrices
	auto worldMat = XMMatrixTranspose(world);
	auto viewMat = XMMatrixTranspose(view);
	auto projectMat = XMMatrixTranspose(project);

	// Set matrices parameter
	// Lock the constant buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = D3DClass::GetInstance().GetDeviceContext()->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr))
		return false;

	// Get data pointer
	MaxtrixBufferType* matrixData;
	matrixData = (MaxtrixBufferType*)mappedResource.pData;

	// Copy the matrices
	matrixData->world = worldMat;
	matrixData->view = viewMat;
	matrixData->projection = projectMat;

	// Unlock the constant buffer
	D3DClass::GetInstance().GetDeviceContext()->Unmap(m_matrixBuffer, 0);

	// Set constant buffer in the vertex shader
	D3DClass::GetInstance().GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

	// Set light parameter
	// Lock the constant buffer
	hr = D3DClass::GetInstance().GetDeviceContext()->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr))
		return false;

	// Get data pointer
	LightBufferType* lightData;
	lightData = (LightBufferType*)mappedResource.pData;

	// Copy the data
	lightData->color = color;
	lightData->direction = direction;
	if(shaderRsrcView)
	{
		lightData->useTex = true;
		// Set shader texture resource in the pixel shader
		D3DClass::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &shaderRsrcView);
	}
	else
		lightData->useTex = false;

	lightData->viewDir = viewDirection;
	lightData->padding = 0.0f;

	// Unlock the constant buffer
	D3DClass::GetInstance().GetDeviceContext()->Unmap(m_lightBuffer, 0);

	// Set material parameter
	// Lock the constant buffer
	hr = D3DClass::GetInstance().GetDeviceContext()->Map(m_materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr))
		return false;

	// Get data pointer
	MaterialType* materialData;
	materialData = (MaterialType*)mappedResource.pData;

	// Copy data
	materialData->ambient = material.ambient;
	materialData->diffuse = material.diffuse;
	materialData->specular = material.specular;
	materialData->specularPower = material.specularPower;

	// Unlock the constant buffer
	D3DClass::GetInstance().GetDeviceContext()->Unmap(m_materialBuffer, 0);

	// Set shader texture resource in the pixel shader
	D3DClass::GetInstance().GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_lightBuffer);
	D3DClass::GetInstance().GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_materialBuffer);
	
	return true;
}

void LightShader::RenderShader(const int indexCount)
{
	// Set the vertex input layout
	D3DClass::GetInstance().GetDeviceContext()->IASetInputLayout(m_layout);

	// Set the vertex and pixel shader
	D3DClass::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	D3DClass::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	// Set the sampler state
	D3DClass::GetInstance().GetDeviceContext()->PSSetSamplers(0, 1, &m_samplerState);

	// Render
	D3DClass::GetInstance().GetDeviceContext()->DrawIndexed(indexCount, 0, 0);
}

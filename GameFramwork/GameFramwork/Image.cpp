#include "Image.h"
#include "D3DClass.h"
#include "WinMain.h"

Image::Image()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_texture = nullptr;
	m_screenWidth = 0;
	m_screenHeight = 0;
	m_imageWidth = 0;
	m_imageHeight = 0;
	m_prePosX = 0;
	m_prePosY = 0;
}

Image::Image(const Image & image)
{
}

Image::~Image()
{
}

bool Image::Initialize(TCHAR * imagePath)
{
	bool result;

	// Store the screen size
	m_screenWidth = WindowMain::GetInstance().GetScreenWidth();
	m_screenHeight = WindowMain::GetInstance().GetScreenHeight();

	// Setup the previous rendering position
	m_prePosX = -1;
	m_prePosY = -1;

	// Load texture
	result = LoadTexture(imagePath);
	if(!result)
		return false;

	// Initialize the buffers
	result = InitializeBuffers();
	if(!result)
		return false;

	return true;
}

void Image::ShutDown()
{
	// Release the texture
	ReleaseTexture();

	// ShutDown buffers
	ShutdownBuffer();

}

bool Image::Render(int posX, int posY)
{
	bool result;

	// update the dynamic vertex buffer for rendering to possibly a different location on the screen
	result = UpdateBuffer(posX, posY);
	if(!result)
		return false;

	// Put the vertex and index buffers on the graphics pipeline to prepare them for rendering
	RenderBuffers();


	return true;
}

int Image::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView * Image::GetTexture()
{
	return m_texture->GetTexture();
}

bool Image::InitializeBuffers()
{
	HRESULT hr;

	// Set the number of vertices and indices to 6 because a square include 2 triangles
	m_vertexCount = 6;
	m_indexCount = 6;

	// Create vertices array
	ImageVertexType* vertices = new ImageVertexType[m_vertexCount];
	if(!vertices)
		return false;

	// Create indices array
	unsigned long* indices = new unsigned long[m_indexCount];
	if(!indices)
		return false;

	// Initialize the vertex array and index array
	memset(vertices, 0, sizeof(ImageVertexType)*m_vertexCount);
	for(int i = 0; i < m_indexCount; ++i)
		indices[i] = i;

	// Setup the description of the dynamic vertex buffer
	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_DYNAMIC;
	vbDesc.ByteWidth = sizeof(ImageVertexType)*m_vertexCount;
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	hr = D3DClass::GetInstance().GetDevice()->CreateBuffer(&vbDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(hr))
		return false;

	// Setup the description of index buffer.
	D3D11_BUFFER_DESC indexDesc;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(unsigned long)*m_indexCount;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	hr = D3DClass::GetInstance().GetDevice()->CreateBuffer(&indexDesc, &indexData, &m_indexBuffer);
	if(FAILED(hr))
		return false;

	// Release the arrays
	delete[] vertices;
	vertices = nullptr;
	
	delete[] indices;
	indices = nullptr;

	return true;
}

void Image::ShutdownBuffer()
{

	// Release the vertex buffer
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = nullptr;
	}

	// Release the index buffer
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
}

bool Image::UpdateBuffer(int posX, int posY)
{
	HRESULT hr;

	// If the position has not been changed then don`t update
	if((posX == m_prePosX) && (posY == m_prePosY))
	{
		return true;
	}

	// If the position has been changed
	// Update position
	m_prePosX = posX;
	m_prePosY = posY;

	// Create the Rectangle of the image
	Rect imgRect;
	imgRect.left = static_cast<float>(((m_screenWidth / 2)*-1) + static_cast<float>(posX));
	imgRect.top = static_cast<float>((m_screenHeight / 2) - static_cast<float>(posY));
	imgRect.right = imgRect.left + static_cast<float>(m_imageWidth);
	imgRect.bottom = imgRect.top - static_cast<float>(m_imageHeight);

	// Create the vertex array
	ImageVertexType* vertices = new ImageVertexType[m_vertexCount];
	if(!vertices)
		return false;

	// Setup the vertex array
	vertices[0].pos = XMFLOAT3(imgRect.left, imgRect.top, 0.0f);
	vertices[0].tex = XMFLOAT2(0.0f, 0.0f);
	vertices[1].pos = XMFLOAT3(imgRect.right, imgRect.bottom, 0.0f);
	vertices[1].tex = XMFLOAT2(1.0f, 1.0f);
	vertices[2].pos = XMFLOAT3(imgRect.left, imgRect.bottom, 0.0f);
	vertices[2].tex = XMFLOAT2(0.0f, 1.0f);
	vertices[3].pos = XMFLOAT3(imgRect.left, imgRect.top, 0.0f);
	vertices[3].tex = XMFLOAT2(0.0f, 0.0f);
	vertices[4].pos = XMFLOAT3(imgRect.right, imgRect.top, 0.0f);
	vertices[4].tex = XMFLOAT2(1.0f, 0.0f);
	vertices[5].pos = XMFLOAT3(imgRect.right, imgRect.bottom, 0.0f);
	vertices[5].tex = XMFLOAT2(1.0f, 1.0f);

	// Lock the vertex buffer
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = D3DClass::GetInstance().GetDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(hr))
		return false;

	// Get data pointer
	ImageVertexType* imgVerData;
	imgVerData = (ImageVertexType*)mappedResource.pData;

	// Copy the data
	memcpy(imgVerData, vertices, (sizeof(ImageVertexType)*m_vertexCount));

	// Unlock the constant buffer
	D3DClass::GetInstance().GetDeviceContext()->Unmap(m_vertexBuffer, 0);

	// Release the vertex array
	delete[] vertices;
	vertices = nullptr;

	return true;
}

void Image::RenderBuffers()
{
	// Set vertex buffer stride and offset

	UINT strides = sizeof(ImageVertexType);
	UINT offsets = 0;

	// Set the vertex buffer to active 
	D3DClass::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &strides, &offsets);
	// Set the index buffer to active
	D3DClass::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive tha should be rendered from this vertex buffer
	D3DClass::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool Image::LoadTexture(TCHAR * texPath)
{
	bool result;

	// Create the texture object
	m_texture = new Texture;
	if(!m_texture)
		return false;

	result = m_texture->Initialize(texPath);
	if(!result)
		return false;

	// Store the texture size
	m_imageWidth = m_texture->GetWidth();
	m_imageHeight = m_texture->GetHeight();

	return true;
}

void Image::ReleaseTexture()
{
	// Release the texture object
	if(m_texture)
	{
		m_texture->ShutDown();
		delete m_texture;
		m_texture = nullptr;
	}
}

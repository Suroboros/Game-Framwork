/////////////////////////////////////
// Object.cpp
// The mesh of object.
/////////////////////////////////////
#include "Mesh.h"

Mesh::Mesh()
{
	
}

Mesh::Mesh(const Mesh &)
{
}

Mesh::~Mesh()
{
}

bool Mesh::InitializeBuffers(ID3D11Device * device)
{
	VertexType* vertices;
	unsigned long* indices;
	//ID3D11Buffer *vertexBuffer[2] = {nullptr,nullptr}, *indexBuffer;
	
	HRESULT hr;

	
	// Setup the description of the vertex buffer.
	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(mesh.v)*mesh.v.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &mesh.v;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	hr = device->CreateBuffer(&vbDesc, &vertexData, &vertexBuffer[0]);
	if(FAILED(hr))
		return false;

	// Setup the description of the vertex texture Coordinate buffer.
	D3D11_BUFFER_DESC uvDesc;
	uvDesc.Usage = D3D11_USAGE_DEFAULT;
	uvDesc.ByteWidth = sizeof(mesh.vt)*mesh.vt.size();
	uvDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	uvDesc.CPUAccessFlags = 0;
	uvDesc.MiscFlags = 0;
	uvDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex texture Coordinate data.
	D3D11_SUBRESOURCE_DATA vtData;
	vtData.pSysMem = &mesh.vt;
	vtData.SysMemPitch = 0;
	vtData.SysMemSlicePitch = 0;

	// Create vertex texture Coordinate buffer
	hr = device->CreateBuffer(&uvDesc, &vtData, &vertexBuffer[1]);
	if(FAILED(hr))
		return false;

	// Setup the description of index buffer.
	D3D11_BUFFER_DESC indexDesc;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(mesh.faces)*mesh.faces.size();
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &mesh.faces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	hr = device->CreateBuffer(&indexDesc, &indexData, &indexBuffer);
	if(FAILED(hr))
		return false;

	return true;
}

void Mesh::ShutdownBuffer()
{
	// Release the index buffer
	
	if(!indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
	

	// Release the vertex buffer
	if(!vertexBuffer[0])
	{
		vertexBuffer[0]->Release();
		vertexBuffer[0] = nullptr;
	}
	if(!vertexBuffer[1])
	{
		vertexBuffer[1]->Release();
		vertexBuffer[1] = nullptr;
	}
	return;
}

void Mesh::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	// Set vertex buffer stride and offset
	UINT strides[2] = { sizeof(MeshData::v),sizeof(MeshData::vt) };
	UINT offsets[2] = { 0,0 };

	// Set the vertex buffer to active 
		deviceContext->IASetVertexBuffers(0, 2, vertexBuffer, strides, offsets);
	// Set the index buffer to active
		deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive tha should be rendered from this vertex buffer
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

Model::Model()
{
}

Model::Model(Model & model)
{
}

Model::~Model()
{
}

bool Model::Initialize(ID3D11Device * device, string meshPath)
{
	bool result;

	// Load mesh
	result = LoadFromObj(meshPath,device);
	if(!result)
		return false;

	// Initialize the vertex and index buffers.
	for(auto mesh : model)
	{
		result = mesh->InitializeBuffers(device);
		if(!result)
			return false;
	}
	return true;

}

void Model::Shutdown()
{
	for(auto mesh : model)
	{
		mesh->ShutdownBuffer();
		delete mesh;
		mesh = nullptr;
	}

	return;
}

void Model::Render(ID3D11DeviceContext * deviceContext)
{
	for(auto mesh:model)
		mesh->RenderBuffers(deviceContext);

	return;
}

bool Model::LoadFromObj(string filePath,ID3D11Device * device)
{
	ifstream fin;
	string input1, input2;
	Mesh* m =new Mesh;

	XMFLOAT3 v, vn;
	XMFLOAT2 vt;
	XMINT3 index;
	Face f;

	// Open file
	fin.open(filePath, ios::in);
	if(fin.fail())
	{
		MessageBox(nullptr, L"Can't open mesh file", L"Error", MB_OK);
		return false;
	}

	// Read data
	while(!fin.eof() && fin.peek() != EOF)
	{
		fin >> input1;

		if(input1.compare("g") == 0)
		{
			fin >> input1;
			m->mesh.name = input1;

			while(input1.compare("g") != 0 && fin.peek() != EOF || m->mesh.usemtl.empty())
			{
				fin >> input1;
				// Read material name
				if(input1.compare("usemtl") == 0)
					fin >> m->mesh.usemtl;

				// Read geometric vertex
				if(input1.compare("v") == 0)
				{
					fin >> v.x >> v.y >> v.z;
					// Invert the Z vertex to change to left hand system
					v.z = v.z*-1.0f;
					m->mesh.v.push_back(v);

				}

				// Read vertex texture Coordinate Indices
				if(input1.compare("vt") == 0)
				{
					fin >> vt.x >> vt.y;
					// Invert the V texture coordinates to left hand system.
					vt.y = 1.0f - vt.y;
					m->mesh.vt.push_back(vt);
				}

				// Read vertex normal
				if(input1.compare("vn") == 0)
				{
					fin >> vn.x >> vn.y >> vn.z;
					// Invert the Z normal to left hand system.
					vn.z = vn.z*-1.0f;
					m->mesh.vn.push_back(vn);
				}

				// Read face
				if(input1.compare("f") == 0)
				{
					while(fin.peek() != '\n')
					{
						fin >> input2;
						divideIndexString(index, input2);
						f.indices.insert(f.indices.begin(), index);
					}
					m->mesh.faces.push_back(f);
					f.indices.clear();
				}
				fin.get();

			}
			//mesh.InitializeBuffers(device);
			model.push_back(m);
/*			mesh.mesh.name = "";
			mesh.mesh.usemtl = "";
			mesh.mesh.v.clear();
			mesh.mesh.vn.clear();
			mesh.mesh.vt.clear();
			mesh.mesh.faces.clear();
*/			fin.unget();
			fin.unget();
		}

	}

	fin.close();

	return true;
}

void Model::divideIndexString(XMINT3 & index, string input)
{
	char num[3][256];
	int i = 0, j = 0;
	for(auto ch : input)
	{
		if(ch != '/')
		{
			num[j][i] = ch;
			++i;
		}
		else
		{
			num[j][i] = '\0';
			++j;
			i = 0;
		}
	}
	num[j][i] = '\0';
	index.x = atoi(num[0]);
	index.y = atoi(num[1]);
	index.z = atoi(num[2]);
}

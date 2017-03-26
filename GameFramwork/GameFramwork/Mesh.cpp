/////////////////////////////////////
// Object.cpp
// The mesh of object.
/////////////////////////////////////
#include "Mesh.h"

Mesh::Mesh()
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}

Mesh::Mesh(const Mesh &)
{
}

Mesh::~Mesh()
{
}

bool Mesh::InitializeBuffers(ID3D11Device * device)
{
	//VertexDataType* vertices;
	//unsigned long* indices;
	//ID3D11Buffer *vertexBuffer[2] = {nullptr,nullptr}, *indexBuffer;
	
	HRESULT hr;

	
	// Setup the description of the vertex buffer.
	D3D11_BUFFER_DESC vbDesc;
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(VertexDataType)*data.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;
	vbDesc.MiscFlags = 0;
	vbDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = &(data[0]);
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	hr = device->CreateBuffer(&vbDesc, &vertexData, &vertexBuffer);
	if(FAILED(hr))
		return false;

	// Setup the description of index buffer.
	D3D11_BUFFER_DESC indexDesc;
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(unsigned int)*indices.size();
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &(indices[0]);
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
	
	if(indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
	

	// Release the vertex buffer
	if(vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}

	return;
}

void Mesh::RenderBuffers(ID3D11DeviceContext * deviceContext)
{
	// Set vertex buffer stride and offset

	UINT strides = sizeof(VertexDataType);
	UINT offsets = 0;

	// Set the vertex buffer to active 
		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &strides, &offsets);
	// Set the index buffer to active
		deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	// Set the type of primitive tha should be rendered from this vertex buffer
		deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

Model::Model()
{
	texture = nullptr;
}

Model::Model(Model & model)
{
}

Model::~Model()
{
}

bool Model::Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext, TCHAR* meshPath, TCHAR* texPath)
{
	bool result;

	// Load mesh
	result = LoadFromObj(meshPath);
	if(!result)
		return false;

	// Load texture if texture is setted
	if(texPath)
	{
		result = LoadTexture(device, deviceContext, texPath);
		if(!result)
			return false;
	}

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
	// Release texture
	ReleaseTexture();

	// Release meshes
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
//	model[0]->RenderBuffers(deviceContext);
	return;
}

int Model::GetIndexCount()
{
	return 36;
}

ID3D11ShaderResourceView * Model::GetTexture()
{
	if(texture)
		return texture->GetTexture();
	else
		return nullptr;
}

bool Model::LoadFromObj(TCHAR* filePath)
{
	ifstream fin;
	string input, input2;

	int gCount = 0;
	int vCount = 0;
	int vtCount = 0;
	int vnCount = 0;
	int fCount = 0;
	XMFLOAT3 v, vn;
	XMFLOAT2 vt;
	XMINT3 vertex;
	Face f;
	GroupInfo gInfo;


	// Open file
	fin.open(filePath, ios::in);
	if(fin.fail())
	{
		MessageBox(nullptr, _T("Can't open mesh file"), _T("Error"), MB_OK);
		return false;
	}

	// Read data
	while(!fin.eof() && fin.peek() != EOF)
	{

		fin >> input;

		// Read mtl file path
		if(input.compare("mtllib") == 0)
			fin >> objData.mtllib;

		// Read geometric vertex
		if(input.compare("v") == 0)
		{
			fin >> v.x >> v.y >> v.z;
			// Invert the Z vertex to change to left hand system
			v.z = v.z*-1.0f;
			objData.v.push_back(v);

		}

		// Read vertex texture Coordinate Indices
		if(input.compare("vt") == 0)
		{
			fin >> vt.x >> vt.y;
			// Invert the V texture coordinates to left hand system.
			vt.y = 1.0f - vt.y;
			objData.vt.push_back(vt);
		}

		// Read vertex normal
		if(input.compare("vn") == 0)
		{
			fin >> vn.x >> vn.y >> vn.z;
			// Invert the Z normal to left hand system.
			vn.z = vn.z*-1.0f;
			objData.vn.push_back(vn);
		}

		// Read group information
		if(input.compare("g") == 0)
		{
			string g;
			fin >> g >> input;

			if(input.compare("usemtl") != 0)
			{
				fin.unget();
				continue;
			}

			gInfo.name = g;
			// Read material name
			fin >> gInfo.usemtl;

			//  Read face
			fin.get();
			input = (char)fin.peek();
			while(input.compare("f") == 0)
			{
				fin >> input;
				while(fin.peek() != '\n')
				{
					fin >> input2;
					DivideIndexString(vertex, input2);
					f.vertices.insert(f.vertices.begin(), vertex);
				}
				gInfo.faces.push_back(f);
				f.vertices.clear();
				fin.get();
				input = (char)fin.peek();
			}

			objData.groups.push_back(gInfo);
			gInfo.name = "";
			gInfo.usemtl = "";
			gInfo.faces.clear();
		}
	}
	// Close file
	fin.close();

	// Write to mesh
	if(!WriteMesh())
		return false;
	return true;
}

void Model::DivideIndexString(XMINT3 & index, string input)
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

void Model::ReleaseTexture()
{
	if(texture)
	{
		texture->ShutDown();
		delete texture;
		texture = 0;
	}
}

bool Model::WriteMesh()
{
	// Write mtl file path
	mtlPath = objData.mtllib;

	// Write vertex information
	for(auto group : objData.groups)
	{
		int vIndex = 0, vtIndex = 0, vnIndex = 0;
		Mesh* mesh = new Mesh;
		// Write group name and material name
		mesh->name = group.name;
		mesh->usemtl = group.usemtl;
		unsigned int idxCnt = 0;
		// Write face information
		for(auto f : group.faces)
		{
			vector<int> iList({ 0,1,2 });
			// If the face has 4 vertices
			if(f.vertices.size() == 4)
			{
				vector<int> sub({ 0,2,3 });
				iList.insert(iList.end(), sub.begin(), sub.end());
			}
			
			for(auto i : iList)
			{

				vIndex = f.vertices[i].x - 1;
				vtIndex = f.vertices[i].y - 1;
				vnIndex = f.vertices[i].z - 1;

				VertexDataType vd;
				// Write vertex
				vd.pos = objData.v[vIndex];
				if(vtIndex < 0) // If no texture coordinate data
					vd.tex = { 0.0,0.0 };
				else
					vd.tex = objData.vt[vtIndex];
				vd.nor = objData.vn[vnIndex];
				mesh->data.push_back(vd);
				// Write index
				
				mesh->indices.push_back(idxCnt);
				++idxCnt;
			}
		}

		model.push_back(mesh);
	}
	if(model.empty())
		return false;

	return true;
}

bool Model::LoadTexture(ID3D11Device * device, ID3D11DeviceContext * deviceContext, TCHAR * texPath)
{
	bool result;

	// Create the texture object
	texture = new Texture;
	if(!texture)
		return false;

	result = texture->Initialize(device, deviceContext, texPath);
	if(!result)
		return false;
	return false;
}

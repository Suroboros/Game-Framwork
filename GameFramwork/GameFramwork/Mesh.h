#pragma once
/////////////////////////////////////
// Mesh.h
// Loading, unloading, and accessing of a mesh resource
/////////////////////////////////////

#ifndef _MESH_H_
#define _MESH_H_

// Include
#include <fstream>
#include <string>
#include <vector>
#include <d3d11.h>
#include <directxmath.h>
#include <DirectXMath.h>
#include <Windows.h>
using namespace std;
using namespace DirectX;

// Obj format data
// Face
struct Face
{
	vector<XMINT3> indices;
};
// The data struct of mesh
struct MeshData
{
	// Group name
	string name;
	// Material name
	string usemtl;
	// Geometric vertices
	vector<XMFLOAT3> v;
	// Texture coordinates
	vector<XMFLOAT2> vt;
	// Vertex normal
	vector<XMFLOAT3> vn;
	// Face
	vector<Face> faces;
};


// Mesh class
class Mesh
{
	//The definition of vertex type that will be used with the vertex buffer in this ModelClass.
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	Mesh();
	Mesh(const Mesh&);
	~Mesh();


	MeshData mesh;

	// Initialize the vertex and index buffers.
	bool InitializeBuffers(ID3D11Device* device);
	// Release the vertex buffer and index buffer.
	void ShutdownBuffer();
	// Setup the vertex buffer and index buffer.
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* vertexBuffer[2] = { nullptr,nullptr };
	ID3D11Buffer* indexBuffer;
	
	
	
};

// Mesh container
class Model
{
public:
	Model();
	Model(Model& model);
	~Model();

	vector<Mesh*> model;

	bool Initialize(ID3D11Device* device, string meshPath);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	// Load obj format mesh file
	bool LoadFromObj(string filePath, ID3D11Device * device);

private:
	// Divide the index from the f string
	void divideIndexString(XMINT3& index, string input);
};




#endif // !_MESH_H_
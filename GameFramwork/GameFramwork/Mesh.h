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
#include <tchar.h>
#include "Texture.h"
using namespace std;
using namespace DirectX;

// //The definition of vertex type that will be used with the vertex buffer in this ModelClass.
struct VertexDataType
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
	XMFLOAT3 nor;

};

// Mesh class
class Mesh
{

public:
	Mesh();
	Mesh(const Mesh&);
	~Mesh();

	// Group name
	string name;
	// Material name
	string usemtl;
	// Vertices data
	vector<VertexDataType> data;
	// Index data
	vector<unsigned int> indices;

	// Initialize the vertex and index buffers.
	bool InitializeBuffers(ID3D11Device* device);
	// Release the vertex buffer and index buffer.
	void ShutdownBuffer();
	// Setup the vertex buffer and index buffer.
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
		
};

// Mesh container
class Model
{
	// Obj format data
	// Face
	struct Face
	{
		vector<XMINT3> vertices;
	};
	// Group information
	struct GroupInfo
	{
		string name;
		// Material name
		string usemtl;
		// Face
		vector<Face> faces;
	};
	// Data structure
	struct OBJDataType
	{
		string mtllib;
		// Geometric vertices
		vector<XMFLOAT3> v;
		// Texture coordinates
		vector<XMFLOAT2> vt;
		// Vertex normal
		vector<XMFLOAT3> vn;
		// Face
		vector<GroupInfo> groups;
	};
public:
	Model();
	Model(Model& model);
	~Model();

	vector<Mesh*> model;

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, TCHAR* meshPath, TCHAR* texPath = nullptr);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	// Load obj format mesh file
	bool LoadFromObj(TCHAR* filePath);

private:
	// Obj data
	OBJDataType objData;
	// Material file path
	string mtlPath;
	Texture* texture;

	// Divide the index from the f string
	void DivideIndexString(XMINT3& index, string input);
	// Write obj data to mesh
	bool WriteMesh();

	bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, TCHAR* texPath);
	void ReleaseTexture();

};




#endif // !_MESH_H_
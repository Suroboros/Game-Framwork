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

// The definition of vertex type that will be used with the vertex buffer in this ModelClass.
struct VertexDataType
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
	XMFLOAT3 nor;

};

// The definition of material type
struct MaterialType
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT3 specular;
	float specularPower;
};


// Mesh class
class Mesh
{

public:
	Mesh();
	Mesh(const Mesh&);
	~Mesh();

	// Group name
	string m_name;
	// Material name
	string m_usemtl;
	// Vertices data
	vector<VertexDataType> m_data;
	// Index data
	vector<unsigned int> m_indices;
	// Material
	MaterialType m_meterial;

	// Initialize the vertex and index buffers.
	bool InitializeBuffers();
	// Release the vertex buffer and index buffer.
	void ShutdownBuffer();
	// Setup the vertex buffer and index buffer.
	void RenderBuffers();

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
		
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

	vector<Mesh*> m_model;
	float m_width, m_height, m_depth;

	bool Initialize(TCHAR* meshPath, TCHAR* texPath = nullptr);
	void Shutdown();
	void Render();

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	

private:
	// Obj data
	OBJDataType m_objData;
	// Material file path
	string m_mtlPath;
	Texture* m_texture;

	// Divide the index from the f string
	void DivideIndexString(XMINT3& index, string input);
	// Load obj format mesh file
	bool LoadFromObj(TCHAR* filePath);
	// Load mtl file
	bool LoadMtl();
	// Write obj data to mesh
	bool WriteMesh();

	bool LoadTexture(TCHAR* texPath);
	void ReleaseTexture();

};




#endif // !_MESH_H_
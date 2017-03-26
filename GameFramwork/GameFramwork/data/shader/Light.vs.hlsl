/////////////////////////////////////
// Light.vs
/////////////////////////////////////

// Globals
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
}

// Type define
struct VertexInputType
{
	float3 position:POSITION;
	float2 tex:TEXCOORD;
	float3 normal:NORMAL;
};

struct PixelInputType
{
	float4 position:SV_POSITION;
	float2 tex:TEXCOORD;
	float3 normal:NORMAL;
};

// Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
	PixelInputType output;
	
	// Set the position vector to be 4 units
	float4 pos4 = float4(input.position, 1.0f);
	
	// Coordinate transformation
	output.position=mul(pos4,worldMatrix);
	output.position=mul(output.position,viewMatrix);
	output.position=mul(output.position,projectionMatrix);
	
	// Store the texture coordinates
	output.tex=input.tex;
	
	// Calculate the normal vector
	output.normal=mul(input.normal,(float3x3)worldMatrix);
	
	// Normalize
	output.normal=normalize(output.normal);
	
	return output;
}
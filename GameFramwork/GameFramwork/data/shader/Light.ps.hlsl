/////////////////////////////////////
// Light.ps
/////////////////////////////////////

// Globals
Texture2D shaderTexture;
SamplerState samplerType;

cbuffer LightBuffer
{
	float4 lightColor;
	float3 lightDirection;
	float padding;
	bool useTexture;
}

// Type define
struct PixelInputType
{
	float4 position:SV_POSITION;
	float2 tex:TEXCOORD;
	float3 normal:NORMAL;
};

// Pixel Shader
float4 LightPixelShader(PixelInputType input):SV_TARGET
{
	float4 texColor;
	float3 dir;
	float lightIntensity;
	float4 color;
	
	// Sample the pixel color from texture
	//if(useTexture)
	//	texColor=shaderTexture.Sample(samplerType,input.tex);
	//else
		texColor= float4(1,1,1,1);
	
	// Invert the light direction
	dir=-lightDirection;
	
	// Calculate the amount of light on this pixel
	lightIntensity=saturate(dot(input.normal,dir));
	
	// Calculate the final amount of the color
	color=saturate(lightColor*lightIntensity);
	
	// Multiply the texture pixel and the color
	color=color*texColor;
	
	return color;
}
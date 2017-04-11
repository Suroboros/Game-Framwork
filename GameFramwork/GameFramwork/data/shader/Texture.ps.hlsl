/////////////////////////////////////
// Texture.ps
/////////////////////////////////////

// Globals
Texture2D shaderTexture;
SamplerState samplerType;

// Type define
struct PixelInputType
{
	float4 position:SV_POSITION;
	float2 tex:TEXCOORD;
};

// Pixel Shader
float4 TexturePixelShader(PixelInputType input) :SV_TARGET
{
	float4 texColor;

	// Sample the pixel color from texture
	texColor = shaderTexture.Sample(samplerType, input.tex);

	return texColor;
}
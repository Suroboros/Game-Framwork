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
	bool useTexture;
	float3 viewDirection;
	float padding;
};

// Meterial buffer
cbuffer MaterialBuffer
{
	float4 diffuseColor;
	float4 ambientColor;
	float3 specularColor;
	float specularPow;
	
};

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
	float4 diffuse;
	float4 ambient;
	float3 reflection;
	float4 specular;
	
	// Sample the pixel color from texture
	if(useTexture)
		texColor = shaderTexture.Sample(samplerType, input.tex);
	else 
		texColor = float4(1, 1, 1, 1);
	
	// Set the diffuse color
	if(length(diffuseColor) != 0)
		diffuse = diffuseColor;
	else
		diffuse = lightColor;

	// Set ambient color
	ambient = float4(ambientColor.r*lightColor.r, ambientColor.g*lightColor.g, ambientColor.b*lightColor.b, lightColor.a);

	// Initialize the specular color
	specular = float4(specularColor.r, specularColor.g, specularColor.b, 1);

	// Invert the light direction
	dir=-lightDirection;
	
	// Calculate the amount of light on this pixel
	lightIntensity = max(dot(normalize(input.normal), normalize(dir)), 0);
	
	//
//	if(lightIntensity > 0.0f)
	{
		// Calculate the final diffuse color
		diffuse = diffuse*saturate(lightIntensity);

		// Calculate the reflection vector
		//reflection = normalize(2 * lightIntensity*normalize(input.normal) - normalize(dir));
		reflection = reflect(lightDirection, input.normal);
		reflection = normalize(reflection);
		// Calculate specular light
		float specularLight = pow(saturate(max(dot(normalize(viewDirection), reflection),0)), specularPow);
		if(lightIntensity <= 0)specularLight = 0;
		//specularLight = 0;
		specular = specular*specularLight;

	}

	// Saturate the color
	//color = saturate(color);
	
	// Multiply the texture pixel and the color
	color = (ambient + diffuse + specular)*texColor;

	// Add the specular light
	//color = saturate(color + specular);
	
	return color;
}
#pragma once
/////////////////////////////////////
// Effect.h
// Maintein some state in the pipeline
/////////////////////////////////////

#ifndef _EFFECT_H_
#define _EFFECT_H_

// Include
#include <d3d11.h>

// Effect class
class Effect
{
public:
	Effect();
	Effect(const Effect& effect);
	~Effect();

	bool Initialize();
	void Shutdown();

	// Make depth buffer valid
	void DepthBufferOn();
	// Make depth buffer invalid
	void DepthBufferOff();

private:
	
	// Default depth setting for 3D rendering
	ID3D11DepthStencilState* depthStencilState3D;
	// Default depth setting for 2D rendering
	ID3D11DepthStencilState* depthStencilState2D;

};

#endif // !_EFFECT_H_
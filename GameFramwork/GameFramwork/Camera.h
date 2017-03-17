#pragma once
/////////////////////////////////////
// Camera.cpp
// Make DirectX know from where and also how we are viewing the scene.
/////////////////////////////////////

#ifndef _CAMERA_H_
#define _CAMERA_H_

// Include
#include <memory>
#include "GEUtility.h"

// Camera class
class Camera
{
public:
	Camera();
	Camera(const XMFLOAT3& pos, const XMFLOAT3& focus, const XMFLOAT3& up, float angle, float aspRadio, float nearZ, float farZ);
	Camera(const Camera&);
	~Camera();

	// Setup the position, target and rotation of the camera.
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetFocus(float x, float y, float z);

	// Get the position, target and rotarion of the camera.
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetFocus();

	// Use the position and rotation of the camera to setup and update camera.
	void Update();
	// Get the updated view matrix after the update function has been called.
	void GetViewMatrix(XMMATRIX &viewMatrix);

	using spCamera = std::shared_ptr<Camera>;
	static spCamera Create(const XMFLOAT3& pos, const XMFLOAT3& focus, const XMFLOAT3& up, float angle, float aspRadio, float nearZ, float farZ);
private:
	XMFLOAT3 position, rotation, up, focus;
	XMFLOAT4X4 viewMat, projMat;
	float fovAngle, aspRadio, nearZ, farZ;

};

#endif // !_CAMERA_H_
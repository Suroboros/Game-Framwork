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
	struct ViewFrustum
	{
		// The boundaries of the view frustum.
		XMFLOAT4 plants[6];
	};
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	bool Initialize(const XMFLOAT3& pos, const XMFLOAT3& focus, const XMFLOAT3& up, float angle, float aspRadio, float nearZ, float farZ);
	void Shutdown();

	// Setup the position, target, rotation and up of the camera.
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetFocus(float x, float y, float z);
	void SetUp(float x, float y, float z);
	// Set angel of view
	void SetAOV(float angleOfView);

	// Check that whether the object is in the view frustum or not.
	// Check point.
	bool CheckFrustumP(Point pos);
	// Check cuboid.
	bool CheckFrustumC(Box cuboid);
	// Check sphere.
	bool CheckFrustumS(Point center, float radius);

	// Get the position, target and rotarion of the camera.
	Point GetPosition();
	XMFLOAT3 GetRotation();
	XMFLOAT3 GetFocus();

	// Get the direction of view
	XMFLOAT3 GetViewDirection();

	// Use the position and rotation of the camera to setup and update camera.
	void Update();
	// Get the updated view matrix after the update function has been called.
	void GetViewMatrix(XMMATRIX &viewMatrix);
	// Get the updated projection matrix after the update function has been called.
	void GetProjectionMatrix(XMMATRIX &projMatrix);
	// Get the ortho matrix.
	void GetOrthoMatrix(XMMATRIX &orthoMatrix);

private:
	XMFLOAT3 m_position, m_rotation, m_up, m_focus;
	XMFLOAT4X4 m_viewMat, m_projMat, m_orthoMat;
	float m_fovAngle, m_aspRadio, m_nearZ, m_farZ;
	ViewFrustum m_viewFrustum;
};

#endif // !_CAMERA_H_
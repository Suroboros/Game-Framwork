#include "Camera.h"
/////////////////////////////////////
// Camera.cpp
// Make DirectX know from where and also how we are viewing the scene.
/////////////////////////////////////

Camera::Camera()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	focus = XMFLOAT3(0.0f, 0.0f, 0.0f);
	fovAngle = 0.0f;
	aspRadio = 0.0f;
	nearZ = 0.0f;
	farZ = 0.0f;
}

Camera::Camera(const XMFLOAT3& pos, const XMFLOAT3& focus, const XMFLOAT3& up, float angle, float aspRadio, float nearZ, float farZ)
{
	position = pos;
	this->focus = focus;
	this->up = up;
	fovAngle = angle;
	this->aspRadio = aspRadio;
	this->nearZ = nearZ;
	this->farZ = farZ;
}
Camera::Camera(const Camera &)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
}

void Camera::SetRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
}

void Camera::SetFocus(float x, float y, float z)
{
	focus = XMFLOAT3(x, y, z);
}

XMFLOAT3 Camera::GetPosition()
{
	return position;
}

XMFLOAT3 Camera::GetRotation()
{
	return rotation;
}

XMFLOAT3 Camera::GetFocus()
{
	return focus;
}

void Camera::Update()
{
	XMVECTOR posVec, upVec, focusVec;
	XMMATRIX viewMatrix, projMatrix;

	posVec = XMLoadFloat3(&position);
	upVec = XMLoadFloat3(&up);
	focusVec = XMLoadFloat3(&focus);

	// Calculate the view matrix.
	viewMatrix = XMMatrixLookAtLH(posVec, focusVec, upVec);
	
	// Calculate the projection matrix.
	projMatrix = XMMatrixPerspectiveFovLH(fovAngle, aspRadio, nearZ, farZ);

	XMStoreFloat4x4(&viewMat, viewMatrix);
	XMStoreFloat4x4(&projMat, projMatrix);

}

void Camera::GetViewMatrix(XMMATRIX &viewMatrix)
{
	viewMatrix = XMLoadFloat4x4(&viewMat);
}

Camera::spCamera Camera::Create(const XMFLOAT3& pos, const XMFLOAT3& focus, const XMFLOAT3& up, float angle, float aspRadio, float nearZ, float farZ)
{
	return Camera::spCamera(new Camera(pos, focus,up, angle, aspRadio, nearZ, farZ));
}

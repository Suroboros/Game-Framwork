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
/*	XMVECTOR posVec, upVec, focusVec;
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
*/

	XMFLOAT3 u, pos, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	pos.x = position.x;
	pos.y = position.y;
	pos.z = position.z;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = position.x * 0.0174532925f;
	yaw = position.y * 0.0174532925f;
	roll = position.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	XMStoreFloat3(&lookAt, XMVector3TransformCoord(XMLoadFloat3(&lookAt), rotationMatrix));
	XMStoreFloat3(&u, XMVector3TransformCoord(XMLoadFloat3(&u), rotationMatrix));

	// Translate the rotated camera position to the location of the viewer.
	XMStoreFloat3(&lookAt, XMLoadFloat3(&pos) + XMLoadFloat3(&lookAt));

	// Finally create the view matrix from the three updated vectors.
	XMStoreFloat4x4(&viewMat, XMMatrixLookAtLH(XMLoadFloat3(&position), XMLoadFloat3(&lookAt), XMLoadFloat3(&up)));

	return;
}

void Camera::GetViewMatrix(XMMATRIX &viewMatrix)
{
	viewMatrix = XMLoadFloat4x4(&viewMat);
}

Camera::spCamera Camera::Create(const XMFLOAT3& pos, const XMFLOAT3& focus, const XMFLOAT3& up, float angle, float aspRadio, float nearZ, float farZ)
{
	return Camera::spCamera(new Camera(pos, focus,up, angle, aspRadio, nearZ, farZ));
}

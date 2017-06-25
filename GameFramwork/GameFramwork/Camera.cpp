#include "Camera.h"
#include "WinMain.h"
/////////////////////////////////////
// Camera.cpp
// Make DirectX know from where and also how we are viewing the scene.
/////////////////////////////////////

Camera::Camera()
{
	m_position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_focus = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_fovAngle = 0.0f;
	m_aspRadio = 0.0f;
	m_nearZ = 0.0f;
	m_farZ = 0.0f;
}

Camera::Camera(const Camera &)
{
}

Camera::~Camera()
{
}

bool Camera::Initialize(const XMFLOAT3 & pos, const XMFLOAT3 & focus, const XMFLOAT3 & up, float angle, float aspRadio, float nearZ, float farZ)
{
	m_position = pos;
	m_focus = focus;
	m_up = up;
	m_fovAngle = angle;
	m_aspRadio = aspRadio;
	m_nearZ = nearZ;
	m_farZ = farZ;

	// Create view frustum.
	// Calculate the view matrix.
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&m_position), XMLoadFloat3(&m_focus), XMLoadFloat3(&m_up));

	// Calculate the projection matrix.
	XMMATRIX projMatrix = XMMatrixPerspectiveFovLH(m_fovAngle, m_aspRadio, m_nearZ, m_farZ);

	// Calculate the ortho matrix.
	XMMATRIX orthoMatrix = XMMatrixOrthographicLH(static_cast<float>(WindowMain::GetInstance().GetScreenWidth()), static_cast<float>(WindowMain::GetInstance().GetScreenHeight()), m_nearZ, m_farZ);

	XMStoreFloat4x4(&m_viewMat, viewMatrix);
	XMStoreFloat4x4(&m_projMat, projMatrix);
	XMStoreFloat4x4(&m_orthoMat, orthoMatrix);

	// Create the frustum matrix from the view matrix and updated projection matrix.
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixMultiply(viewMatrix, projMatrix));

	XMVECTOR plant;
	
	// Calculate near plane of frustum.
	plant = XMVectorSet(matrix._14 + matrix._13, matrix._24 + matrix._23, matrix._34 + matrix._33, matrix._44 + matrix._43);
	XMStoreFloat4(&m_viewFrustum.plants[0], XMPlaneNormalize(plant));

	// Calculate far plane of frustum.
	plant = XMVectorSet(matrix._14 - matrix._13, matrix._24 - matrix._23, matrix._34 - matrix._33, matrix._44 - matrix._43);
	XMStoreFloat4(&m_viewFrustum.plants[1], XMPlaneNormalize(plant));

	// Calculate left plane of frustum.
	plant = XMVectorSet(matrix._14 + matrix._11, matrix._24 + matrix._21, matrix._34 + matrix._31, matrix._44 + matrix._41);
	XMStoreFloat4(&m_viewFrustum.plants[2], XMPlaneNormalize(plant));

	// Calculate right plane of frustum.
	plant = XMVectorSet(matrix._14 - matrix._11, matrix._24 - matrix._21, matrix._34 - matrix._31, matrix._44 - matrix._41);
	XMStoreFloat4(&m_viewFrustum.plants[3], XMPlaneNormalize(plant));

	// Calculate top plane of frustum.
	plant = XMVectorSet(matrix._14 - matrix._12, matrix._24 - matrix._22, matrix._34 - matrix._32, matrix._44 - matrix._42);
	XMStoreFloat4(&m_viewFrustum.plants[4], XMPlaneNormalize(plant));

	// Calculate bottom plane of frustum.
	plant = XMVectorSet(matrix._14 + matrix._12, matrix._24 + matrix._22, matrix._34 + matrix._32, matrix._44 + matrix._42);
	XMStoreFloat4(&m_viewFrustum.plants[5], XMPlaneNormalize(plant));

	return true;
}

void Camera::Shutdown()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT3(x, y, z);
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotation = XMFLOAT3(x, y, z);
}

void Camera::SetFocus(float x, float y, float z)
{
	m_focus = XMFLOAT3(x, y, z);
}

void Camera::SetUp(float x, float y, float z)
{
	m_up = XMFLOAT3(x, y, z);
}

void Camera::SetAOV(float angleOfView)
{
	m_fovAngle = angleOfView;
}

bool Camera::CheckFrustumP(Point pos)
{
	for(int i = 0; i < 6; ++i)
	{
		if(Point2PlaneDis(m_viewFrustum.plants[i],pos) < 0.0f)
			return false;
	}
	return true;
}

bool Camera::CheckFrustumC(Box cuboid)
{
	Point point;
	for(int i = 0; i < 6; ++i)
	{
		point.x = cuboid.center.x - cuboid.width / 2.0f;
		point.y = cuboid.center.y - cuboid.height / 2.0f;
		point.z = cuboid.center.z - cuboid.depth / 2.0f;
		if(Point2PlaneDis(m_viewFrustum.plants[i], point) >= 0.0f)
			continue;

		point.x = cuboid.center.x + cuboid.width / 2.0f;
		point.y = cuboid.center.y - cuboid.height / 2.0f;
		point.z = cuboid.center.z - cuboid.depth / 2.0f;
		if(Point2PlaneDis(m_viewFrustum.plants[i], point) >= 0.0f)
			continue;

		point.x = cuboid.center.x - cuboid.width / 2.0f;
		point.y = cuboid.center.y + cuboid.height / 2.0f;
		point.z = cuboid.center.z - cuboid.depth / 2.0f;
		if(Point2PlaneDis(m_viewFrustum.plants[i], point) >= 0.0f)
			continue;

		point.x = cuboid.center.x - cuboid.width / 2.0f;
		point.y = cuboid.center.y - cuboid.height / 2.0f;
		point.z = cuboid.center.z + cuboid.depth / 2.0f;
		if(Point2PlaneDis(m_viewFrustum.plants[i], point) >= 0.0f)
			continue;

		point.x = cuboid.center.x + cuboid.width / 2.0f;
		point.y = cuboid.center.y + cuboid.height / 2.0f;
		point.z = cuboid.center.z - cuboid.depth / 2.0f;
		if(Point2PlaneDis(m_viewFrustum.plants[i], point) >= 0.0f)
			continue;

		point.x = cuboid.center.x + cuboid.width / 2.0f;
		point.y = cuboid.center.y - cuboid.height / 2.0f;
		point.z = cuboid.center.z + cuboid.depth / 2.0f;
		if(Point2PlaneDis(m_viewFrustum.plants[i], point) >= 0.0f)
			continue;

		point.x = cuboid.center.x - cuboid.width / 2.0f;
		point.y = cuboid.center.y + cuboid.height / 2.0f;
		point.z = cuboid.center.z + cuboid.depth / 2.0f;
		if(Point2PlaneDis(m_viewFrustum.plants[i], point) >= 0.0f)
			continue;

		point.x = cuboid.center.x + cuboid.width / 2.0f;
		point.y = cuboid.center.y + cuboid.height / 2.0f;
		point.z = cuboid.center.z + cuboid.depth / 2.0f;
		if(Point2PlaneDis(m_viewFrustum.plants[i], point) >= 0.0f)
			continue;

		return false;
	}
	return true;
}

bool Camera::CheckFrustumS(Point center, float radius)
{
	for(int i = 0; i < 6; ++i)
	{
		if(Point2PlaneDis(m_viewFrustum.plants[i], center) < -radius)
			return false;
	}
	return false;
}

Point Camera::GetPosition()
{
	Point pos;
	pos.x = m_position.x;
	pos.y = m_position.y;
	pos.z = m_position.z;
	return pos;
}

XMFLOAT3 Camera::GetRotation()
{
	return m_rotation;
}

XMFLOAT3 Camera::GetFocus()
{
	return m_focus;
}

XMFLOAT3 Camera::GetViewDirection()
{
	XMFLOAT3 viewDir;
	XMStoreFloat3(&viewDir, XMLoadFloat3(&m_position) - XMLoadFloat3(&m_focus));
	return viewDir;
}



void Camera::Update()
{
	XMVECTOR posVec, upVec, focusVec;
	XMMATRIX viewMatrix, projMatrix;

	posVec = XMLoadFloat3(&m_position);
	upVec = XMLoadFloat3(&m_up);
	focusVec = XMLoadFloat3(&m_focus);

	// Calculate the view matrix.
	viewMatrix = XMMatrixLookAtLH(posVec, focusVec, upVec);
	
	// Calculate the projection matrix.
	projMatrix = XMMatrixPerspectiveFovLH(m_fovAngle, m_aspRadio, m_nearZ, m_farZ);

	XMStoreFloat4x4(&m_viewMat, viewMatrix);
	XMStoreFloat4x4(&m_projMat, projMatrix);

	return;
}

void Camera::GetViewMatrix(XMMATRIX &viewMatrix)
{
	viewMatrix = XMLoadFloat4x4(&m_viewMat);
}

void Camera::GetProjectionMatrix(XMMATRIX & projMatrix)
{
	projMatrix = XMLoadFloat4x4(&m_projMat);
}

void Camera::GetOrthoMatrix(XMMATRIX & orthoMatrix)
{
	orthoMatrix = XMLoadFloat4x4(&m_orthoMat);
}

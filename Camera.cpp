#include "Camera.h"



Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::InitCamera()
{
	horizontal_fov = 90;

	frustum.horizontalFov = DegToRad(90);
	frustum.verticalFov = DegToRad(60);
	frustum.nearPlaneDistance = 0.5;
	frustum.farPlaneDistance = 500;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.SetWorldMatrix(float3x4::identity);

	frustum_culling = false;
}

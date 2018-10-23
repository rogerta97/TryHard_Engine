#include "Camera.h"



Camera::Camera()
{

}


Camera::~Camera()
{
}

float* Camera::GetProjectionMatrix()
{
	static float4x4 m;

	m = frustum.ProjectionMatrix();
	m.Transpose();

	return (float*)m.v;
}

void Camera::InitCamera()
{
	projection_changed = true;

	horizontal_fov = 90;

	frustum.horizontalFov = DegToRad(90);
	frustum.verticalFov = DegToRad(60);
	frustum.nearPlaneDistance = 0.5;
	frustum.farPlaneDistance = 25;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.SetWorldMatrix(float3x4::identity);

	frustum_culling = false;
}

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

	aspect_ratio = 1.5f;
	SetFOV(DegToRad(60));
		
	frustum.nearPlaneDistance = 0.5;
	frustum.farPlaneDistance = 25;

	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.SetWorldMatrix(float3x4::identity);

	frustum_culling = false;
}

void Camera::SetFOV(float new_v_fov)
{
	frustum.verticalFov = new_v_fov;
	frustum.horizontalFov = math::Atan(aspect_ratio * math::Tan(frustum.verticalFov / 2)) * 2;
}

void Camera::SetAspectRatio(float new_ar)
{
	aspect_ratio = new_ar;
	frustum.horizontalFov = math::Atan(aspect_ratio * math::Tan(frustum.verticalFov / 2)) * 2;
}

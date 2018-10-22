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


	frustum.SetViewPlaneDistances(5.0f, 500.0f);
	frustum.SetKind(FrustumProjectiveSpace::FrustumSpaceGL,FrustumHandedness::FrustumLeftHanded);
	frustum.SetWorldMatrix(float3x4::identity);
	frustum.SetPerspective(DegToRad(90), DegToRad(60));

	//float4x4 matrix = frustum.ComputeProjectionMatrix();

	//frustum.horizontalFov = DegToRad(90);
	//frustum.verticalFov = DegToRad(60);
	//frustum.nearPlaneDistance = 0.5;
	//frustum.farPlaneDistance = 25;

	//frustum.type = FrustumType::PerspectiveFrustum;


	frustum_culling = false;
}

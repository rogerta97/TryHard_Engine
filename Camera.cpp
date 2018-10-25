#include "Camera.h"
#include "GameObject.h"


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

Frustum_Collision Camera::IsAABBInside(const AABB &box)
{

	float3 vCorner[8];
	Plane planes[6];
	frustum.GetPlanes(planes);
	int iTotalIn = 0;
	box.GetCornerPoints(vCorner);
	// get the corners of the box into the vCorner array
	// test all 8 corners against the 6 sides
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (planes[p].IsOnPositiveSide(vCorner[i])) {
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		if (iInCount == 0)
			return(OUTSIDE_FRUSTUM);
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
		return(INSIDE_FRUSTUM);
	// we must be partly
	return(INTERSECTS_FRUSTUM);
}

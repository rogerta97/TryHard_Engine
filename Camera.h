#pragma once
#include "Resource.h"
#include "MathGeoLib\MathGeoLib.h"

enum Frustum_Collision { INSIDE_FRUSTUM, OUTSIDE_FRUSTUM, INTERSECTS_FRUSTUM };

enum Camera_Aspect
{
	ASP_EDITOR,
	ASP_MAIN,
	ASP_NONE,
};

class Camera
{
public:
	Camera();
	~Camera();

	float* GetProjectionMatrix();

	void InitCamera(); 

	void SetFOV(float new_fov_v); 
	float GetFov() const;
	void SetAspectRatio(float new_ar);

	Frustum_Collision IsAABBInside(const AABB &box);

	Frustum frustum; 

	float horizontal_fov;
	float vertical_fov;

	float dock_aspect_ratio;
	float aspect_ratio;

	bool projection_changed;

	Camera_Aspect aspect; 

	float2 size;
};


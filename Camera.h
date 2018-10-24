#pragma once
#include "Resource.h"
#include "MathGeoLib\MathGeoLib.h"

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
	void SetAspectRatio(float new_ar);

	Frustum frustum; 

	float horizontal_fov;
	float vertical_fov;

	float aspect_ratio;

	bool projection_changed;

	bool frustum_culling;

	Camera_Aspect aspect; 

	float2 size;
};


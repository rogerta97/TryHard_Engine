#pragma once
#include "Resource.h"
#include "MathGeoLib\MathGeoLib.h"

enum Projection_Type
{
	PROJ_PERSP,
	PROJ_ORTH,
};

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

	void InitCamera(); 

	Frustum frustum; 

	float horizontal_fov;
	float vertical_fov; 
	float aspect_ratio;

	bool frustum_culling;

	Projection_Type projection; 
	Camera_Aspect aspect; 

	float2 size;
};


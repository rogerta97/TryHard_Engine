#pragma once

#include "Globals.h"
#include "Component.h"
#include "glmath.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Timer.h"

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

class TextureMSAA;

struct CameraInterpolation
{
	Timer interpolation_timer;
	uint interpolation_ms;

	//Position
	LineSegment line;

	//Rotation
	vec3 source_vec;
	vec3 dst_vec;

	vec center;

	bool interpolate;

	void Init()
	{
		interpolation_ms = 350.0f;
		source_vec = { 0,0,0 };
		dst_vec = { 0,0,0 };
		interpolate = false;
	}
};

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	~ComponentCamera();

	bool Start(); 
	bool Update(); 
	bool CleanUp(); 

	//Movement
	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	//Interpolation
	void FillInterpolationSegmentAndRot();
	bool InterpolateCamera(float time);
	float3 GetCamPointFromDistance(vec center, float distance) const;

	//Setters & Getters
	void SetSpeed(float new_speed);
	float GetSpeed() const;

	void SetMouseSensitivity(float new_sensitivity);
	float GetMouseSensitivity() const;

	Projection_Type GetProjection() const;
	void SetProjection(Projection_Type type); 

	Camera_Aspect GetAspect() const;
	void SetAspect(Camera_Aspect type);

	//Utility
	void LockCamera();
	void UnlockCamera();
	bool IsLocked() const;

	void CalculateViewMatrix();

	//Docking Texture
	TextureMSAA* GetViewportTexture();

public: 

	vec3 X, Y, Z, Position, Reference;
	CameraInterpolation interpolation; 
	float wheel_zoom_speed = 5.0f;
	float speed_multiplier;
	float mouse_sensitivity = 5.0f;

private: 

	mat4x4 ViewMatrix, ViewMatrixInverse;

	Projection_Type projection; 
	Camera_Aspect aspect; 

	TextureMSAA* viewport_texture;
	float speed = 0.1f;

	bool locked;
	bool orbit = true;
};


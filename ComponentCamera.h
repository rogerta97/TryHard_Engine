#pragma once

#include "Globals.h"
#include "Component.h"
#include "glmath.h"
#include "MathGeoLib\MathGeoLib.h"
#include "Timer.h"

enum Projection_Type 
{ 
	PROJ_ORTH,
	PROJ_PERSP
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

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void FillInterpolationSegmentAndRot();
	bool InterpolateCamera(float time);
	float3 GetCamPointFromDistance(vec center, float distance) const;

	void SetSpeed(float new_speed);
	float GetSpeed() const;

	void SetMouseSensitivity(float new_sensitivity);
	float GetMouseSensitivity() const;

	TextureMSAA* GetViewportTexture();

	void LockCamera();
	void UnlockCamera();
	bool IsLocked() const;

	void CalculateViewMatrix();

public: 

	vec3 X, Y, Z, Position, Reference;
	CameraInterpolation cam_interpolation;
	float wheel_zoom_speed = 5.0f;

private: 

	mat4x4 ViewMatrix, ViewMatrixInverse;
	Projection_Type projection; 

	TextureMSAA* viewport_texture;

	float speed = 0.1f;
	float mouse_sensitivity = 5.0f;
	
	float speed_multiplier;
	bool locked;

	bool orbit = true;

};


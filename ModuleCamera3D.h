#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Timer.h"
#include "MathGeoLib\MathGeoLib.h"

class TextureMSAA; 

struct CameraInterpolation
{
	void Init()
	{
		interpolation_ms = 1000.0f; 
		source_vec = { 0,0,0 }; 
		dst_vec = { 0,0,0 };
		interpolate = false; 
	}

	Timer interpolation_timer; 
	uint interpolation_ms; 

	//Position
	LineSegment line; 

	//Rotation
	vec3 source_vec; 
	vec3 dst_vec; 

	bool interpolate; 
};

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void PrintConfigData();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void FillInterpolationSegmentAndRot(); 
	bool InterpolateCamera(float time);
	float3 GetCamPointFromDistance(vec center, float distance);

	void SetSpeed(float new_speed); 
	float GetSpeed() const;

	void SetMouseSensitivity(float new_sensitivity);
	float GetMouseSensitivity() const;

	TextureMSAA* GetViewportTexture(); 

	void LockCamera(); 
	void UnlockCamera(); 
	bool IsLocked(); 

private:

	void CalculateViewMatrix();


public:
	
	vec3 X, Y, Z, Position, Reference;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
	TextureMSAA* viewport_texture; 

	float speed = 0.1f;
	float mouse_sensitivity = 5.0f;
	float speed_multiplier;
	bool locked;

	bool orbit = true;

	CameraInterpolation cam_interpolation;

};
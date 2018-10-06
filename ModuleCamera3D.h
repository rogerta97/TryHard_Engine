#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "MathGeoLib\MathGeoLib.h"

class TextureMSAA; 
class ComponentBoundingBox; 

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

	void LookAtSelectedGameObject(); 
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
	bool locked;

	bool orbit = true;
};
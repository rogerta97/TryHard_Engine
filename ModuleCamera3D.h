#pragma once
#include "Module.h"
#include "Globals.h"
#include "Timer.h"
#include "SkyBox.h"

#include "ComponentCamera.h"
#include "GameObject.h"

class TextureMSAA; 

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void PrintConfigData();

	ComponentCamera* GetEditorCamera(); 

	void ManageMovementFromTrans(ComponentCamera* cam);

	void ManageMovement();

	void MoveRotateECamFrustum(float dt);

	void RotateFrustum(float dx, float dy);

	void MoveFrustum(float dt);

public:
		
	SkyBox* skybox = nullptr; 

	bool frustum_culling;

	float3 Rotate(const float3 &u, float angle, const float3 &v);
	GameObject* GetCameraGO() const;
	
private:

	GameObject* ecam_go; 
};
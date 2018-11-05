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
	ComponentCamera* GetGameCamera();
	void SetGameCamera(GameObject* new_cam);

	bool IsGhostCamera() const;
	void SetGhostCamera(bool value);

	void ManageMovement();

	void DrawMouseRay() const;


public:
		
	SkyBox* skybox = nullptr; 

	bool frustum_culling;

	float3 Rotate(const float3 &u, float angle, const float3 &v);
	GameObject* GetCameraGO() const;
	
private:

	GameObject* ecam_go; 
	GameObject* gcam_go; 

	bool is_ghost_camera;

	bool draw_mouse_picking_ray;

	LineSegment mouse_picking_ray;
};
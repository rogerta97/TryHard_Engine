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

public:
		
	SkyBox* skybox = nullptr; 
	
private:

	GameObject* ecam_go; 

	float3 Rotate(const float3 &u, float angle, const float3 &v);
};
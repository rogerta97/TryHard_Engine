#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

struct RenderSettings
{
	bool depth_test = false;
	bool cull_face = false;
	bool wireframe = false;
	bool color_material = true;
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);
	void SetUIPrintSettings(); 

	char* GetGraphicsModel();
	char* GetGraphicsVendor();

	void SetDefaultRenderSettings(); 
	RenderSettings GetDefaultRenderSettings(); 

	void PrintConfigData(); 

public:

	Light lights[MAX_LIGHTS];
	RenderSettings render_settings; 
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};
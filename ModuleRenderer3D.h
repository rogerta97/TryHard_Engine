#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "MathGeoLib\MathGeoLib.h"
#include <list>

#define MAX_LIGHTS 8

class ComponentCamera; 

struct RenderSettings
{
	bool depth_test = false;
	bool cull_face = false;
	bool wireframe = false;
	bool color_material = true;
	bool texture = true; 
	bool wireframe_selected = true; 
	bool light = true;
};

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_Object* config);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void UseCurrentRenderSettings();
	void UseUIRenderSettings(); 
	void UseDebugRenderSettings(); 

	char* GetGraphicsModel() const;
	char* GetGraphicsVendor() const;


	RenderSettings GetDefaultRenderSettings() const; 

	void AddRenderCamera(ComponentCamera* cam);
	//void SetRenderCamera(ComponentCamera* cam); 
	//ComponentCamera* GetRenderCamera() const;

	void PrintConfigData(); 

public:

	Light lights[MAX_LIGHTS];
	RenderSettings render_settings; 
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;


	std::list<ComponentCamera*> rendering_cameras; 
};
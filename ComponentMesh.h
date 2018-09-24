#pragma once

#include "Component.h"
#include "Mesh.h"

struct RenderSettings
{
	bool depth_test = false; 
	bool cull_face = false; 
	bool wireframe = false; 
	bool color_material = true; 
};

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	bool Update();

	void SetMesh(Mesh* new_mesh);
	void SetDrawSettings(); 
	void SetDefaultSettings();

	void PrintRenderSettings(); 

private:

	Mesh * mesh;
	RenderSettings render_settings; 
	
	bool draw_mesh; 
	bool render_settings_modified; 
};


#pragma once

#include "Component.h"
#include "ModuleRenderer3D.h"
#include "Mesh.h"

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


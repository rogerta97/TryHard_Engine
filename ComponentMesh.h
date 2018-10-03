#pragma once

#include "Component.h"
#include "ModuleRenderer3D.h"
#include "Mesh.h"

class ComponentMaterial; 

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	bool Update();
	void DrawMesh(); 

	void SetMesh(Mesh* new_mesh);
	void SetDrawSettings(); 
	void SetDefaultSettings();

	void PrintRenderSettings(); 

	void AssignMaterial(ComponentMaterial* new_mat);
	ComponentMaterial* GetMaterial() const;

	Mesh* GetMesh() const; 

private:

	Mesh * mesh;
	ComponentMaterial* material; 
	
	bool draw_mesh; 
	bool wireframe; 
};


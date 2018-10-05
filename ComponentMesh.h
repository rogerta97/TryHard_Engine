#pragma once

#include "Component.h"
#include "ModuleRenderer3D.h"
#include "Mesh.h"

class ComponentMaterial; 
class ComponentBoundingBox; 

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	bool Update();

public: 

	void DrawMesh(); 
	void DrawNormals();

	void SetMesh(Mesh* new_mesh);
	void SetDrawSettings(); 
	void SetDefaultSettings();

	void PrintRenderSettings(); 
	 
	void AssignMaterial(ComponentMaterial* new_mat);
	ComponentMaterial* GetMaterial() const;

	Mesh* GetMesh() const; 

	AABB bounding_box; 
	bool CreateEnclosedMeshAABB();
	void DrawBoundingBox(); 
	void SetBBColor(float r, float g, float b); 

	bool draw_bounding_box;

private:

	Mesh *					mesh;
	ComponentMaterial*		material; 
	
	bool					draw_mesh;
	bool					draw_normals;
	
	bool					wireframe; 
};


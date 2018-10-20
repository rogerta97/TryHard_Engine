#pragma once

#include "Component.h"
#include "ModuleRenderer3D.h"
#include "Mesh.h"

class ComponentMaterial; 

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	bool Update();
	bool CleanUp(); 

public: 

	void DrawMesh(); 
	void DrawNormals();

	void SetMesh(Mesh* new_mesh);
	void SetDefaultSettings();

	void PrintRenderSettings(); 
	 
	void AssignMaterial(ComponentMaterial* new_mat);
	ComponentMaterial* GetMaterial() const;
	void SetMaterial(ComponentMaterial* mat);
	void DeleteMaterial(); 

	void CheckAABBPoints(float3& min_point, float3 &max_point);

	Mesh* GetMesh() const; 

	AABB bounding_box; 
	bool CreateEnclosedMeshAABB();
	void DrawBoundingBox(); 
	void UpdateBoundingBox(); 
	void SetBBColor(float r, float g, float b); 

	bool draw_bounding_box;

private:

	Mesh *					mesh;
	ComponentMaterial*		material; 
	
	bool					draw_mesh;
	bool					draw_normals;
	
	bool					wireframe; 
};


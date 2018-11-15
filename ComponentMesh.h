#pragma once

#include "Component.h"
#include "ModuleRenderer3D.h"
#include "Mesh.h"

class ComponentMaterial; 
enum Frustum_Collision;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* parent);
	~ComponentMesh();

	bool Update();
	bool CleanUp(); 
	void DrawMesh();
	void Draw(bool is_editor); 

public: 

	void DrawNormals();

	void SetMesh(Mesh* new_mesh);
	void SetDefaultSettings();

	void PrintRenderSettings(); 
	 
	void AssignMaterial(ComponentMaterial* new_mat);
	ComponentMaterial* GetMaterial() const;
	void SetMaterial(ComponentMaterial* mat);
	void DeleteMaterial(); 

	void CheckAABBPoints(float3& min_point, float3 &max_point);

	void Load(JSON_Object* root_obj, UID prefab_uid = 0); 
	void Save(JSON_Object* root_obj, const char* root);

	Mesh* GetMesh() const; 

	AABB bounding_box; 
	bool CreateEnclosedMeshAABB();
	void DrawBoundingBox(); 
	void UpdateBoundingBox(); 
	void SetBBColor(float r, float g, float b); 

	bool GetClosestIntersectionPoint(LineSegment line, float3 &closest_point, float & distance);

	bool draw_bounding_box;
	std::string container_fbx; 

	Frustum_Collision		frustum_col_type;

private:

	Mesh *					mesh;
	ComponentMaterial*		material; 


	
	bool					draw_mesh;
	bool					draw_normals;
	
	bool					wireframe; 
};


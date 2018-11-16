#pragma once

#include <list>
#include "Mesh.h"
#include "Globals.h"

#include "Importer.h"

class GameObject; 
class aiScene; 
class aiNode; 
class Mesh; 

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	bool Start();
	bool Update();
	bool CleanUp(); 

	//Basic Geometry Data
	Mesh* CreateCubeMesh(); 
	Mesh* CreatePlaneMesh();
	Mesh* CreateSphereMesh();

	void ImportAllFilesFromAssets();

	void ManageNewItem(std::string new_item_path); 

	std::string GetNameFromMeshMeta(std::string meta_name); 

	void DrawMeshList();

	GameObject* CreateFBXMesh(const char* path, UID root_uid = 0, bool first_load = false); //Returns the set of meshes that form the file
	void LoadFBXMesh(const char* path, aiNode* node, aiScene* scene, GameObject* gameobject_in, bool first_load = false, UID root_uid = 0);

	bool Import(Mesh* saving_mesh, const char* mesh_name, UID parent_mesh_uid = 0);
	Mesh* LoadFromBinary(const char* mesh_name);

};


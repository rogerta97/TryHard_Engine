#pragma once

#include <list>
#include "Mesh.h"
#include "Globals.h"

#include "Importer.h"

class GameObject; 
class aiScene; 
class aiNode; 

class MeshImporter : public Importer
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

	GameObject* CreateFBXMesh(const char* path); //Returns the set of meshes that form the file
	void LoadFBXMesh(const char* path, aiNode* node, aiScene* scene, GameObject* gameobject_in);

	Mesh* GetMeshByType(BasicMeshType type); 

private: 

	std::list<Mesh*> mesh_list; 
};


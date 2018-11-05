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

	void DrawMeshList();

	GameObject* CreateFBXMesh(const char* path); //Returns the set of meshes that form the file
	void LoadFBXMesh(const char* path, aiNode* node, aiScene* scene, GameObject* gameobject_in);

	bool Import(Mesh* saving_mesh, const char* mesh_name);
	Mesh* LoadFromBinary(const char* mesh_name);

};


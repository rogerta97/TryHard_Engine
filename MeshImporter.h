#pragma once

#include <list>
#include "Mesh.h"
#include "Globals.h"

#include "Importer.h"

class GameObject; 

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
	Mesh* CreateShpereMesh();
	std::list<GameObject*> CreateFBXMesh(const char* path); //Returns the set of meshes that form the file

	Mesh* GetMeshByType(BasicMeshType type); 

private: 

	std::list<Mesh*> mesh_list; 
};


#pragma once

#include <list>
#include "Mesh.h"
#include "Globals.h"

#include "Importer.h"



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

	Mesh* GetMeshByType(BasicMeshType type); 

private: 

	std::list<Mesh*> mesh_list; 
};


#include "MeshImporter.h"



MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}

bool MeshImporter::Start()
{
	Mesh* plane_mesh = CreatePlaneMesh();
	mesh_list.push_back(plane_mesh); 

	imp_type = IMP_MESH;

	return true;
}

bool MeshImporter::Update()
{

	return false;
}

bool MeshImporter::CleanUp()
{
	return false;
}

Mesh * MeshImporter::CreateCubeMesh()
{
	return nullptr;
}

Mesh * MeshImporter::CreatePlaneMesh()
{
	Mesh* new_plane_mesh = new Mesh(); 

	new_plane_mesh->CreateBuffer(); 
	new_plane_mesh->BindBuffer(); 
	new_plane_mesh->SetPlaneData(); 
	new_plane_mesh->UnbindBuffer();

	return new_plane_mesh;
}

Mesh * MeshImporter::GetMeshByType(BasicMeshType type)
{
	for (auto it = mesh_list.begin(); it != mesh_list.end(); it++)
	{
		if (type == (*it)->GetType())
		{
			return (*it);
		}
	}

	return nullptr;
}

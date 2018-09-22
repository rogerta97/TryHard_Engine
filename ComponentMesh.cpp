#include "ComponentMesh.h"



ComponentMesh::ComponentMesh()
{
}


ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update()
{
	if (draw_mesh)
		return false;

	mesh->DrawMesh();

	return true; 
}

void ComponentMesh::SetMesh(Mesh * new_mesh)
{
	mesh = new_mesh;
}

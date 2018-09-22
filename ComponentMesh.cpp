#include "ComponentMesh.h"



ComponentMesh::ComponentMesh()
{
	draw_mesh = true; 
}


ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update()
{
	if (draw_mesh == false)
		return false;

	mesh->DrawMesh();

	return true; 
}

void ComponentMesh::SetMesh(Mesh * new_mesh)
{
	mesh = new_mesh;
}

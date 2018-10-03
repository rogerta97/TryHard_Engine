#include "ComponentBoundingBox.h"
#include "GameObject.h"
#include "ComponentMesh.h"

#include "Mesh.h"

ComponentBoundingBox::ComponentBoundingBox()
{
	component_type = CMP_BOUNDINGBOX; 
}

ComponentBoundingBox::~ComponentBoundingBox()
{
}

bool ComponentBoundingBox::Start()
{
	active = true; 

	return true;
}

bool ComponentBoundingBox::Update()
{
	if(active)


	return true;
}

bool ComponentBoundingBox::CleanUp()
{
	return true;
}

void ComponentBoundingBox::DrawBoundingBox()
{

}

bool ComponentBoundingBox::CreateEnclosedMeshAABB()
{
	//Get Vertices of the mesh 
	ComponentMesh* mesh_cmp = (ComponentMesh*)gameobject->GetComponent(CMP_RENDERER);
	
	if (mesh_cmp != nullptr)
	{
		bounding_box.MinimalEnclosingAABB(mesh_cmp->GetMesh()->vertices, mesh_cmp->GetMesh()->num_vertices);
		return true; 
	}
	

	return false;
}

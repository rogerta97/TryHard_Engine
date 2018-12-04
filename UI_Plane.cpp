#include "UI_Plane.h"
#include "Mesh.h"

#include "Application.h"

UI_Plane::UI_Plane()
{
	CreatePlaneMesh(); 
}


UI_Plane::~UI_Plane()
{
}

void UI_Plane::Start()
{
	
}

void UI_Plane::Update()
{
}

void UI_Plane::CleanUp()
{

}

void UI_Plane::CreatePlaneMesh()
{
	quad_plane_mesh = (Mesh*)App->resources->Get(RES_MESH, "Plane");
	quad_plane_mesh->reference_counting++; 
	quad_plane_mesh->SetVertPlaneData();
	quad_plane_mesh->LoadToMemory();
}

Mesh * UI_Plane::GetMesh() const
{
	return quad_plane_mesh;
}

void UI_Plane::SetMesh(Mesh * nValue)
{
	quad_plane_mesh = nValue;
}


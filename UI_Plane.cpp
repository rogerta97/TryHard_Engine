#include "UI_Plane.h"
#include "Mesh.h"

#include "Application.h"

UI_Plane::UI_Plane(float2 size)
{
	quad_plane_mesh = nullptr; 
	CreatePlaneMesh(size);
}

UI_Plane::UI_Plane()
{
	quad_plane_mesh = nullptr;
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

void UI_Plane::CreatePlaneMesh(float2 size)
{
	if (quad_plane_mesh != nullptr)
		quad_plane_mesh->CleanMeshData(); 

	quad_plane_mesh = new Mesh(); /* (Mesh*)App->resources->Get(RES_MESH, "Plane");*/
	quad_plane_mesh->SetVertPlaneData(size);
	quad_plane_mesh->LoadToMemory();
}

void UI_Plane::InvertImage(float2 size)
{
	quad_plane_mesh->CleanMeshData(); 
	quad_plane_mesh->SetVertPlaneData(size);

	quad_plane_mesh->uvs_cords[0] = 0.0f;
	quad_plane_mesh->uvs_cords[1] = 0.0f;
	quad_plane_mesh->uvs_cords[2] = 0.0f;

	quad_plane_mesh->uvs_cords[3] = 1.0f;
	quad_plane_mesh->uvs_cords[4] = 0.0f;
	quad_plane_mesh->uvs_cords[5] = 0.0f;

	quad_plane_mesh->uvs_cords[6] = 0.0f;
	quad_plane_mesh->uvs_cords[7] = 1.0f;
	quad_plane_mesh->uvs_cords[8] = 0.0f;

	quad_plane_mesh->uvs_cords[9] = 1.0f;
	quad_plane_mesh->uvs_cords[10] = 1.0f;
	quad_plane_mesh->uvs_cords[11] = 0.0f;

	quad_plane_mesh->LoadToMemory();
}

void UI_Plane::Resize(float x, float y)
{
	CreatePlaneMesh({ x,y }); 
}

Mesh * UI_Plane::GetMesh() const
{
	return quad_plane_mesh;
}

void UI_Plane::SetMesh(Mesh * nValue)
{
	quad_plane_mesh = nValue;
}


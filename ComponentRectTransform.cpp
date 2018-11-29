#include "ComponentRectTransform.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ComponentMesh.h"
#include "Mesh.h"
#include "OpenGL.h"

ComponentRectTransform::ComponentRectTransform(GameObject* parent)
{
	component_type = CMP_RECTTRANSFORM;
	gameobject = parent;

	transform_part = new ComponentTransform(parent);

	transform_part->transform.position = float3::zero; 
	transform_part->transform.rotation = Quat::identity;
	transform_part->transform.scale = float3::zero;

	CreateRectQuad();

	anchor_point = { 0,0 };
}

ComponentRectTransform::~ComponentRectTransform()
{
	delete transform_part; 
}

bool ComponentRectTransform::Start()
{
	return true;
}

bool ComponentRectTransform::Update()
{
	DrawRectFrame(); 

	return true;
}

bool ComponentRectTransform::CleanUp()
{
	return true;
}

void ComponentRectTransform::AddaptRectToScreenSize()
{
	rect = SDL_Rect({ 0,0,250,250 }); 
}

void ComponentRectTransform::CreateRectQuad()
{
	quad_cmp_mesh = new ComponentMesh(gameobject);

	Mesh* plane_mesh = new Mesh(); 
	plane_mesh->SetVertPlaneData(); 
	plane_mesh->LoadToMemory(); 

	quad_cmp_mesh->SetMesh(plane_mesh);
}

void ComponentRectTransform::DrawRectFrame()
{	
	quad_cmp_mesh->DrawMesh();
}

ComponentTransform* ComponentRectTransform::GetTransform()
{

	return transform_part; 
}

void ComponentRectTransform::SetAnchorPoint(float2 new_point)
{
	anchor_point = new_point; 
}

float2 ComponentRectTransform::GetAnchorPoint() const
{
	return anchor_point;
}

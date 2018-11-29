#include "ComponentRectTransform.h"
#include "ComponentTransform.h"

ComponentRectTransform::ComponentRectTransform(GameObject* parent)
{
	component_type = CMP_RECTTRANSFORM;

	transform_part = new ComponentTransform(parent);

	transform_part->transform.position = float3::zero; 
	transform_part->transform.rotation = Quat::identity;
	transform_part->transform.scale = float3::zero;

	anchor_point = { 0,0 };
}


ComponentRectTransform::~ComponentRectTransform()
{
	delete transform_part; 
}

bool ComponentRectTransform::Start()
{
	return false;
}

bool ComponentRectTransform::Update()
{
	return false;
}

bool ComponentRectTransform::CleanUp()
{
	return false;
}

void ComponentRectTransform::AddaptRectToScreenSize()
{
	rect = SDL_Rect({ 0,0,250,250 }); 
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

#include "ComponentRectTransform.h"
#include "ComponentTransform.h"

ComponentRectTransform::ComponentRectTransform(GameObject* parent)
{
	component_type = CMP_RECTTRANSFORM;

	transform_part = new ComponentTransform(parent);

	transform_part->transform.position = float3::zero; 
	transform_part->transform.rotation = Quat::identity;
	transform_part->transform.scale = float3::zero;

	anchor.rect_transform_attached = this; 
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

ComponentTransform* ComponentRectTransform::GetTransform()
{
	return transform_part; 
}

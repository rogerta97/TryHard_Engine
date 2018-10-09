#include "ComponentTransform.h"



ComponentTransform::ComponentTransform()
{
	transform.position = float3(0.0f, 0.0f, 0.0f);
	transform.rotation = Quat(); 
	transform.scale = float3(0.0f, 0.0f, 0.0f);

	component_type = CMP_TRANSFORM; 
	active = true;
}

ComponentTransform::~ComponentTransform()
{
}

bool ComponentTransform::Start()
{
	return true; 
}

bool ComponentTransform::Update()
{
	return true; 
}

bool ComponentTransform::CleanUp()
{

	return true; 
}

float3 ComponentTransform::GetPosition() const
{
	return transform.position;
}

Quat ComponentTransform::GetRotation() const
{
	return transform.rotation;
}

float3 ComponentTransform::GetScale() const
{
	return transform.scale;
}

void ComponentTransform::SetPosition(float3 new_pos)
{
	transform.position = new_pos; 
}

void ComponentTransform::SetRotation(Quat new_rot)
{
	transform.rotation = new_rot; 
}

void ComponentTransform::SetScale(float3 new_esc)
{
	transform.scale = new_esc;
}


#include "ComponentTransform.h"
#include "Globals.h"


ComponentTransform::ComponentTransform()
{
	transform.position = float3(0.0f, 0.0f, 0.0f);
	transform.rotation = Quat::identity; 
	transform.scale = float3(1.0f, 1.0f, 1.0f);

	CalculateViewMatrix(); 

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

void ComponentTransform::CalculateViewMatrix()
{
	float4x4 new_mat = float4x4::identity;
	new_mat = new_mat * transform.rotation; 
	new_mat = new_mat * new_mat.Scale(transform.scale); 
	new_mat.SetTranslatePart(transform.position); 

	ViewMatrix = new_mat;
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

float4x4 ComponentTransform::GetViewMatrix()
{
	return ViewMatrix;
}

float3 ComponentTransform::GetRotationEuler() const
{
	return transform.rotation.ToEulerXYZ();
}

void ComponentTransform::SetRotationEuler(float3 new_rot)
{
	transform.rotation = Quat::FromEulerXYZ(DEGTORAD*new_rot.x, DEGTORAD*new_rot.y, DEGTORAD*new_rot.z);
}

void ComponentTransform::SetPosition(float3 new_pos)
{
	transform.position = new_pos; 
	CalculateViewMatrix(); 
}

void ComponentTransform::SetRotation(Quat new_rot)
{
	transform.rotation = new_rot; 
	CalculateViewMatrix();
}

void ComponentTransform::SetScale(float3 new_esc)
{
	transform.scale = new_esc;
	CalculateViewMatrix();
}


#include "ComponentTransform.h"
#include "Globals.h"


ComponentTransform::ComponentTransform()
{
	transform.position = float3(0.0f, 0.0f, 0.0f);
	transform.rotation = Quat::identity; 
	transform.scale = float3(1.0f, 1.0f, 1.0f);
	transform.euler_angles = float3::zero; 

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

void ComponentTransform::ResetTransform()
{
	transform.position = { 0,0,0 }; 
	transform.rotation = Quat::identity; 
	transform.scale = float3(1.0f, 1.0f, 1.0f); 

	CalculateViewMatrix(); 
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

float4x4 ComponentTransform::GetGlobalViewMatrix()
{
	float4x4 to_ret_mat = float4x4::identity; 

	return to_ret_mat;
}

float3 ComponentTransform::GetRotationEuler() const
{
	return transform.euler_angles;
}

void ComponentTransform::SetRotationEuler(float3 new_rot)
{
	Quat new_quat = Quat::identity; 

	new_quat = new_quat * new_quat.RotateX(DEGTORAD*new_rot.x);
	new_quat = new_quat * new_quat.RotateY(DEGTORAD*new_rot.y);
	new_quat = new_quat * new_quat.RotateZ(DEGTORAD*new_rot.z);

	transform.rotation = new_quat;
	transform.euler_angles = new_rot; 

	CalculateViewMatrix();
}

void ComponentTransform::SetPosition(float3 new_pos)
{
	transform.position = new_pos; 
	CalculateViewMatrix(); 
}

void ComponentTransform::SetRotation(Quat new_rot)
{
	transform.rotation = new_rot; 
	transform.euler_angles = new_rot.ToEulerXYZ(); 
	CalculateViewMatrix();
}

void ComponentTransform::SetScale(float3 new_esc)
{
	transform.scale = new_esc;
	CalculateViewMatrix();
}


#include "ComponentTransform.h"
#include "Globals.h"
#include "GameObject.h"
#include "OpenGL.h"
#include <string>

ComponentTransform::ComponentTransform(GameObject* parent)
{
	SetGameObject(parent); 
	transform.position = float3(0.0f, 0.0f, 0.0f);
	transform.rotation = Quat::identity; 
	transform.scale = float3(1.0f, 1.0f, 1.0f);
	transform.euler_angles = float3::zero; 

	CalculateViewMatrix(); 

	transform.X = { 1.0f, 0.0f, 0.0f };
	transform.Y = { 0.0f, 1.0f, 0.0f };
	transform.Z = { 0.0f, 0.0f, 1.0f };

	has_transformed = true;
	GlobalMatrix = float4x4::identity;

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
	transform.euler_angles = float3::zero; 

	CalculateViewMatrix(); 
}

void ComponentTransform::CalculateViewMatrix()
{
	float4x4 new_mat = float4x4::identity;
	new_mat = new_mat * new_mat.Scale(transform.scale);
	new_mat = new_mat * transform.rotation; 	
	new_mat.SetTranslatePart(transform.position);

	ViewMatrix = new_mat;

	CalculateGlobalViewMatrix();
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

float4x4 ComponentTransform::GetViewMatrix() //If wanted for OpenGL, will need transposing.
{
	return ViewMatrix;
}

float4x4 ComponentTransform::GetGlobalViewMatrix()
{
	return GlobalMatrix;
}
//	float4x4 to_ret_mat = float4x4::identity; 
//
//	GameObject* current_go = gameobject; 
//
//	do
//	{
//		ComponentTransform* trans = (ComponentTransform*)current_go->GetComponent(CMP_TRANSFORM); 
//
//		to_ret_mat = to_ret_mat * trans->GetViewMatrix(); 
//		current_go = current_go->GetParent();
//
//	/*	if (gameobject->HasChilds()) 
//		{*/
//			to_ret_mat.SetTranslatePart(to_ret_mat.RotatePart() * transform.position);
//			to_ret_mat = transform.rotation.Inverted() * to_ret_mat;
//	//	}
//			
//	} while (current_go != nullptr);
//
//	transform.X = { 1.0f, 0.0f, 0.0f }; 
//	transform.X = to_ret_mat.MulDir(transform.X);
//
//	transform.Y = { 0.0f, 1.0f, 0.0f };
//	transform.Y = to_ret_mat.MulDir(transform.Y);
//
//	transform.Z = { 0.0f, 0.0f, 1.0f };
//	transform.Z = to_ret_mat.MulDir(transform.Z);
//
//	return to_ret_mat;
//;


void ComponentTransform::SetViewMatrix(float4x4 new_mat)
{
	float3 pos = { 0,0,0 };
	Quat rot = { 0,0,0,0 };
	float3 scale = { 0,0,0 };

	float4x4 rotmat = rot.ToFloat4x4();

	new_mat.Transpose();
	new_mat.Inverse();
	new_mat.Decompose(pos, rotmat, scale);

	float3 eulrotmat = rotmat.ToEulerXYZ();

	rot = rot.FromEulerXYZ(eulrotmat.x, eulrotmat.y, eulrotmat.z);

	has_transformed = true;
	transform.position = pos;
	SetRotationEuler(eulrotmat);

	
	//ViewMatrix = new_mat;
}

float3 ComponentTransform::GetRotationEuler() const
{
	return transform.euler_angles;
}

void ComponentTransform::SetRotationEuler(float3 new_rot)
{
	Quat new_quat = Quat::identity; 
	
	new_quat = new_quat * new_quat.RotateY(DEGTORAD*new_rot.y);
	new_quat = new_quat * new_quat.RotateX(DEGTORAD*new_rot.x); 
	new_quat = new_quat * new_quat.RotateZ(DEGTORAD*new_rot.z); 
	
	transform.rotation = new_quat; 
	CalculateViewMatrix();
	transform.euler_angles = new_rot;
}

void ComponentTransform::SetPosition(float3 new_pos)
{
	transform.position = new_pos; 
	CalculateViewMatrix(); 
}

void ComponentTransform::SetRotation(Quat new_rot)
{
	transform.rotation = new_rot; 
	transform.euler_angles = new_rot.ToEulerXYZ() * RADTODEG; 
	CalculateViewMatrix();
}

void ComponentTransform::SetScale(float3 new_esc)
{
	transform.scale = new_esc;
	CalculateViewMatrix();
}

void ComponentTransform::Load(JSON_Object * json_obj)
{
}

void ComponentTransform::Save(JSON_Object * json_obj, const char* root)
{
	std::string node_name = root;
	std::string item_name = "";

	item_name = node_name + ".Components.ComponentTransform.PositionX";
	json_object_dotset_number(json_obj, item_name.c_str(), GetPosition().x);

	item_name = node_name + ".Components.ComponentTransform.PositionY";
	json_object_dotset_number(json_obj, item_name.c_str(), GetPosition().y);

	item_name = node_name + ".Components.ComponentTransform.PositionZ";
	json_object_dotset_number(json_obj, item_name.c_str(), GetPosition().z);

	item_name = node_name + ".Components.ComponentTransform.RotationX";
	json_object_dotset_number(json_obj, item_name.c_str(), GetRotationEuler().x);

	item_name = node_name + ".Components.ComponentTransform.RotationY";
	json_object_dotset_number(json_obj, item_name.c_str(), GetRotationEuler().y);

	item_name = node_name + ".Components.ComponentTransform.RotationZ";
	json_object_dotset_number(json_obj, item_name.c_str(), GetRotationEuler().z);

	item_name = node_name + ".Components.ComponentTransform.ScaleX";
	json_object_dotset_number(json_obj, item_name.c_str(), GetScale().x);

	item_name = node_name + ".Components.ComponentTransform.ScaleY";
	json_object_dotset_number(json_obj, item_name.c_str(), GetScale().y);

	item_name = node_name + ".Components.ComponentTransform.ScaleZ";
	json_object_dotset_number(json_obj, item_name.c_str(), GetScale().z);

}

void ComponentTransform::DrawAxis()
{
	//LineSegment curr_line;

	//glBegin(GL_LINES);
	//App->renderer3D->UseDebugRenderSettings();

	//glLineWidth(8.0f);

	////X
	//curr_line.a = { 0,0,0 };
	//curr_line.b = (0, 0, 0) + X;

	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
	//glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);

	////Y
	//curr_line.a = { 0,0,0 };
	//curr_line.b = float3(0, 0, 0) + Y;

	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
	//glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);

	////Z
	//curr_line.a = { 0,0,0 };
	//curr_line.b = float3(0, 0, 0) + Z;

	//glColor3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
	//glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);

	//glEnable(GL_DEPTH_TEST);
	//glEnd();

	//App->renderer3D->UseCurrentRenderSettings();
}

bool ComponentTransform::HasTransformed()
{
	return has_transformed;
}

void ComponentTransform::SetHasTransformed(bool value)
{
	has_transformed = value;
}

void ComponentTransform::CalculateGlobalViewMatrix()
{
	GlobalMatrix = float4x4::identity;
	GameObject* parent = gameobject->parent;

	if (parent) {
		ComponentTransform* trans = (ComponentTransform*)parent->GetComponent(CMP_TRANSFORM);
		GlobalMatrix = trans->GlobalMatrix * ViewMatrix;
	}
	else
	{
		GlobalMatrix = ViewMatrix;
	}

	
	auto child = gameobject->GetChildList()->begin();
	while (child != gameobject->GetChildList()->end())
	{
			ComponentTransform* child_trans = (ComponentTransform*)(*child)->GetComponent(CMP_TRANSFORM);
			child_trans->CalculateGlobalViewMatrix();
			child++;
	}
	
	has_transformed = true;
	//do {
	//	ComponentTransform* trans = (ComponentTransform*)parent->GetComponent(CMP_TRANSFORM);
	//	GlobalMatrix = trans->GlobalMatrix * ViewMatrix;

	//	parent = gameobject->GetParent();
	//	
	//} while (parent);
}

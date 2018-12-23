#include "ComponentTransform.h"
#include "ComponentRectTransform.h"
#include "Globals.h"
#include "GameObject.h"
#include "OpenGL.h"
#include <string>


#include "mmgr\mmgr.h"


ComponentTransform::ComponentTransform(GameObject* parent)
{
	SetGameObject(parent); 

	transform.position = float3(0.0f, 0.0f, 0.0f);
	transform.rotation = Quat::identity;
	transform.scale = float3(1.0f, 1.0f, 1.0f);
	transform.euler_angles = float3::zero;

	if (parent != nullptr)
	{
		

		CalculateViewMatrix();

		transform.X = { 1.0f, 0.0f, 0.0f };
		transform.Y = { 0.0f, 1.0f, 0.0f };
		transform.Z = { 0.0f, 0.0f, 1.0f };

		has_transformed = true;
		GlobalMatrix = float4x4::identity;
	}


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

void ComponentTransform::CalculateViewMatrixFromGlobal()
{
	GameObject* parent = gameobject->parent;

	if (parent) 
	{
		ComponentTransform* trans = nullptr; 

		if(!parent->GetIsUI())
			trans = (ComponentTransform*)parent->GetComponent(CMP_TRANSFORM);
		else
		{
			ComponentRectTransform* rtrans = (ComponentRectTransform*)parent->GetComponent(CMP_RECTTRANSFORM);
			trans = rtrans->GetTransform(); 
		}

		ViewMatrix = trans->GetGlobalViewMatrix().Inverted() * GlobalMatrix;
	}
	else
		ViewMatrix = GlobalMatrix;

	float3 pos = { 0,0,0 };
	Quat rot = { 0,0,0,0 };
	float3 scale = { 0,0,0 };

	float4x4 rotmat = rot.ToFloat4x4();

	ViewMatrix.Decompose(pos, rotmat, scale);

	float3 eulrotmat = rotmat.ToEulerXYZ();

	SetScale(scale);

	rot = rot.FromEulerXYZ(eulrotmat.x, eulrotmat.y, eulrotmat.z);


	transform.position = pos;
	SetRotation(rot);

	has_transformed = true;

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

float3 ComponentTransform::GetGlobalPosition()
{
	if (gameobject == nullptr)
		return { 0,0,0 }; 

	ComponentTransform* curr_cmp = this; 
	float3 ret_value = { 0,0,0 }; 

	while (curr_cmp->gameobject->GetParent() != nullptr)
	{
		ret_value += curr_cmp->GetPosition();
		curr_cmp = (ComponentTransform*)curr_cmp->gameobject->GetParent()->GetComponent(CMP_TRANSFORM); 
	}

	return ret_value;
}

float4x4 ComponentTransform::GetViewMatrix() //If wanted for OpenGL, will need transposing.
{
	return ViewMatrix;
}

float4x4 ComponentTransform::GetGlobalViewMatrix()
{
	return GlobalMatrix;
}

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

void ComponentTransform::SetGlobalViewMatrix(float4x4 new_mat)
{
	GlobalMatrix = new_mat;

	CalculateViewMatrixFromGlobal();

	CalculateGlobalViewMatrix();
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

void ComponentTransform::SetPositionForUI(float3 new_pos)
{
	float4x4 new_g_matrix = GlobalMatrix;
	new_g_matrix.SetTranslatePart(new_pos);
	SetGlobalViewMatrix(new_g_matrix);
}

void ComponentTransform::Load(JSON_Object * json_obj)
{
	float3 pos = float3::zero; 
	float3 rot = float3::zero;
	float3 scale = float3::zero;
	
	pos.x = json_object_dotget_number(json_obj, "PositionX");
	pos.y = json_object_dotget_number(json_obj, "PositionY");
	pos.z = json_object_dotget_number(json_obj, "PositionZ");

	rot.x = json_object_dotget_number(json_obj, "RotationX");
	rot.y = json_object_dotget_number(json_obj, "RotationY");
	rot.z = json_object_dotget_number(json_obj, "RotationZ");

	scale.x = json_object_dotget_number(json_obj, "ScaleX");
	scale.y = json_object_dotget_number(json_obj, "ScaleY");
	scale.z = json_object_dotget_number(json_obj, "ScaleZ");

	SetPosition(pos); 
	SetRotationEuler(rot); 
	SetScale(scale); 

	//CalculateGlobalViewMatrix(); 
}

void ComponentTransform::Save(JSON_Object * json_obj, const char* root)
{
	std::string node_name = root;
	std::string item_name = "";

	item_name = node_name + ".ComponentTransform.PositionX";
	json_object_dotset_number(json_obj, item_name.c_str(), GetPosition().x);

	item_name = node_name + ".ComponentTransform.PositionY";
	json_object_dotset_number(json_obj, item_name.c_str(), GetPosition().y);

	item_name = node_name + ".ComponentTransform.PositionZ";
	json_object_dotset_number(json_obj, item_name.c_str(), GetPosition().z);

	item_name = node_name + ".ComponentTransform.RotationX";
	json_object_dotset_number(json_obj, item_name.c_str(), GetRotationEuler().x);

	item_name = node_name + ".ComponentTransform.RotationY";
	json_object_dotset_number(json_obj, item_name.c_str(), GetRotationEuler().y);

	item_name = node_name + ".ComponentTransform.RotationZ";
	json_object_dotset_number(json_obj, item_name.c_str(), GetRotationEuler().z);

	item_name = node_name + ".ComponentTransform.ScaleX";
	json_object_dotset_number(json_obj, item_name.c_str(), GetScale().x);

	item_name = node_name + ".ComponentTransform.ScaleY";
	json_object_dotset_number(json_obj, item_name.c_str(), GetScale().y);

	item_name = node_name + ".ComponentTransform.ScaleZ";
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

		ComponentTransform* trans = nullptr;

		if (!parent->GetIsUI())
			trans = (ComponentTransform*)parent->GetComponent(CMP_TRANSFORM);
		else
		{
			ComponentRectTransform* rtrans = (ComponentRectTransform*)parent->GetComponent(CMP_RECTTRANSFORM);
			trans = rtrans->GetTransform();
				
		}

		GlobalMatrix = trans->GlobalMatrix * ViewMatrix;
	}
	else
	{
		GlobalMatrix = ViewMatrix;
	}

	
	auto child = gameobject->GetChildList()->begin();

	while (child != gameobject->GetChildList()->end())
	{
		ComponentTransform* child_trans = nullptr;

		if (!(*child)->GetIsUI())
			child_trans = (ComponentTransform*)(*child)->GetComponent(CMP_TRANSFORM);
		else
		{
			ComponentRectTransform* rtrans = (ComponentRectTransform*)(*child)->GetComponent(CMP_RECTTRANSFORM);
			child_trans = rtrans->GetTransform();
		}

		child_trans->CalculateGlobalViewMatrix();
		child++;
	}
	
	has_transformed = true;
}

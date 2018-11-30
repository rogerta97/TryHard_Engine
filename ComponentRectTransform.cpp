#include "ComponentRectTransform.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "UI_GamePanel.h"
#include "DebugDraw.h"
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
	transform_part->transform.scale = {1,1,1};

	anchor_point = { 0,0 };
}

ComponentRectTransform::~ComponentRectTransform()
{
	delete transform_part; 
}

bool ComponentRectTransform::Start()
{
	CreateRectQuad();

	return true;
}

bool ComponentRectTransform::Update()
{


	return true;
}

bool ComponentRectTransform::CleanUp()
{
	return true;
}

void ComponentRectTransform::Draw(bool is_editor)
{
	DrawRectFrame();
}

void ComponentRectTransform::AddaptRectToScreenSize()
{	
	float2 screen_tex_size = float2(App->imgui->game_panel->GetGameTexSize().x, App->imgui->game_panel->GetGameTexSize().y);
	//float2 screen_tex_size = { 50,50 }; 
	Resize(screen_tex_size);
}

void ComponentRectTransform::CreateRectQuad()
{
	quad_mesh = new Mesh();
	quad_mesh->SetVertPlaneData();
	quad_mesh->LoadToMemory();	
}

void ComponentRectTransform::DrawRectFrame()
{
	App->renderer3D->UseDebugRenderSettings();

	float4x4 view_mat = float4x4::identity;

	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	view_mat.Set((float*)matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)((GetTransform()->GetGlobalViewMatrix()).Transposed() * view_mat).v);
	
	DebugDrawPlane(quad_mesh->vertices, Color(1.0f, 1.0f, 1.0f)); 
	App->renderer3D->GetDefaultRenderSettings();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)view_mat.v);
}

void ComponentRectTransform::Resize(float2 new_size)
{
	float3 curr_rect_scale = GetTransform()->GetScale(); 

	//Get the percentage in which the new_size is bigger/smaller from the current size
	float x = new_size.x / curr_rect_scale.x;
	float y = new_size.y / curr_rect_scale.y;

	float2 scale_percentage = {x,y};

	float3 new_scale = float3(curr_rect_scale.x * scale_percentage.x, curr_rect_scale.y*scale_percentage.y, 1); 
	new_scale /= 4;   // for scale reduction 

	//Scale the canvas
	if (scale_percentage.x != 1 || scale_percentage.y != 1)
	{
		GetTransform()->SetScale(new_scale);
	}
		
	// Set a proper canvas position
	GetTransform()->SetPosition({new_scale.x / 2, new_scale.y / 2, 0 });
												
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

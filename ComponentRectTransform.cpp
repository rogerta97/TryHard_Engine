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
	scale_to_show = { 1,1,1 }; 

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
	if(is_editor)
		DrawRectFrame();
}

void ComponentRectTransform::AddaptRectToScreenSize()
{	
	float2 screen_tex_size = float2(App->imgui->game_panel->GetGameTexSize().x, App->imgui->game_panel->GetGameTexSize().y);
	Resize(screen_tex_size);
}

void ComponentRectTransform::CreateRectQuad()
{
	quad_mesh = new Mesh();
	quad_mesh->SetVertPlaneData();

	width = height = 1; 
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
	float2 half_size = new_size / 2; 

	quad_mesh->vertices[0] = { -half_size.x, half_size.y, 0 };
	quad_mesh->vertices[1] = { half_size.x, half_size.y, 0 };
	quad_mesh->vertices[2] = { -half_size.x, -half_size.y, 0 };
	quad_mesh->vertices[3] = { half_size.x, -half_size.y, 0 };

	width = new_size.x; 
	height = new_size.y;
		
	// Set a proper canvas position
	GetTransform()->SetPosition({ half_size.x, half_size.y, 0 });
												
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

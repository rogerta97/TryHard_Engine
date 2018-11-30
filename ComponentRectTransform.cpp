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

	//In case this is a canvas, set to wireframe 
	//glEnableClientState(GL_VERTEX_ARRAY);
	//
	//glBindBuffer(GL_ARRAY_BUFFER, quad_mesh->vertices_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//
	//glDisable(GL_TEXTURE_2D);
	//glColor3f(0.9f, 0.9f, 0.9f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_mesh->indices_id);
	//glDrawElements(GL_TRIANGLES, quad_mesh->num_indices, GL_UNSIGNED_INT, NULL);
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//
	//glDisableClientState(GL_VERTEX_ARRAY);


}

void ComponentRectTransform::Resize(float2 new_size)
{
	float3 curr_rect_scale = GetTransform()->GetScale(); 

	//Get the percentage in which the new_size is bigger/smaller from the current size
	float x = new_size.x / curr_rect_scale.x;
	float y = new_size.y / curr_rect_scale.y;

	float2 scale_percentage = {x,y};

	//Scale the canvas
	GetTransform()->SetScale(float3(scale_percentage.x, scale_percentage.y, 1));

	// Set a proper canvas position
	//GetTransform()->SetPosition({ GetTransform()->GetPosition().x + new_size.x / 2, GetTransform()->GetPosition().y + new_size.y / 2, 1 });
												
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

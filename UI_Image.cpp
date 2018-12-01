#include "UI_Image.h"
#include "Mesh.h"
#include "OpenGL.h"
#include "Application.h"

#include "DebugDraw.h"
#include "ComponentRectTransform.h"
#include "ComponentTransform.h"

#include "ComponentImage.h"
#include "ModuleRenderer3D.h"

UI_Image::UI_Image(ComponentImage* container)
{
	CreateDrawSpace();
	cmp_container = container; 
}


UI_Image::~UI_Image()
{
}

void UI_Image::Start()
{
	
}

void UI_Image::Update()
{
	
}

void UI_Image::Draw()
{
	DrawImage();
}

void UI_Image::CleanUp()
{
}

void UI_Image::CreateDrawSpace()
{
	draw_space_mesh = new Mesh();
	draw_space_mesh->SetVertPlaneData();
	draw_space_mesh->LoadToMemory();
}

void UI_Image::DrawImage()
{
	App->renderer3D->UseDebugRenderSettings();

	ComponentRectTransform* rtransform = (ComponentRectTransform*)cmp_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);
	ComponentTransform* trans = rtransform->GetTransform();

	float4x4 view_mat = float4x4::identity;

	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	view_mat.Set((float*)matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)((trans->GetGlobalViewMatrix()).Transposed() * view_mat).v);

	DebugDrawPlane(draw_space_mesh->vertices, Color(1.0f, 1.0f, 1.0f));
	App->renderer3D->GetDefaultRenderSettings();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)view_mat.v);

	//ComponentRectTransform* rtransform = (ComponentRectTransform*)cmp_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);
	//ComponentTransform* trans = rtransform->GetTransform();

	//App->renderer3D->GetDefaultRenderSettings(); 

	//float4x4 view_mat = float4x4::identity;

	//if (trans)
	//{
	//	GLfloat matrix[16];
	//	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	//	view_mat.Set((float*)matrix);

	//	glMatrixMode(GL_MODELVIEW);
	//	glLoadMatrixf((GLfloat*)((trans->GetGlobalViewMatrix()).Transposed() * view_mat).v);
	//}

	//glEnableClientState(GL_VERTEX_ARRAY);

	//glBindBuffer(GL_ARRAY_BUFFER, draw_space_mesh->vertices_id);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, draw_space_mesh->indices_id);
	//glDrawElements(GL_TRIANGLES, draw_space_mesh->num_indices, GL_UNSIGNED_INT, NULL);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glDisableClientState(GL_VERTEX_ARRAY);

	//if (trans)
	//{
	//	trans->DrawAxis();
	//	glMatrixMode(GL_MODELVIEW);
	//	glLoadMatrixf((GLfloat*)view_mat.v);
	//}
}

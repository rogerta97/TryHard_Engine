#include "UI_Image.h"
#include "OpenGL.h"
#include "Application.h"

#include "Mesh.h"
#include "Material.h"

#include "DebugDraw.h"
#include "ComponentRectTransform.h"
#include "ComponentTransform.h"

#include "ComponentImage.h"
#include "ModuleRenderer3D.h"

UI_Image::UI_Image(ComponentImage* container)
{
	CreateDrawSpace();
	cmp_container = container; 

	draw_material = (Material*)App->resources->Get(RES_MATERIAL, "DefaultUIBackground");
	draw_material->LoadToMemory();
	draw_material->reference_counting++;
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
	ComponentRectTransform* rtransform = (ComponentRectTransform*)cmp_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);
	ComponentTransform* trans = rtransform->GetTransform();

	App->renderer3D->UseCurrentRenderSettings(); 

	float4x4 view_mat = float4x4::identity;

	if (trans)
	{
		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		view_mat.Set((float*)matrix);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)((trans->GetGlobalViewMatrix()).Transposed() * view_mat).v);
	}

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, draw_space_mesh->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (draw_material != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, draw_space_mesh->uvs_id);

		draw_material->GetDiffuseTexture()->Bind();

		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, draw_space_mesh->indices_id);
	glDrawElements(GL_TRIANGLES, draw_space_mesh->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (draw_material)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY); 

	glDisableClientState(GL_VERTEX_ARRAY);

	if (trans)
	{
		trans->DrawAxis();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)view_mat.v);
	}
}

void UI_Image::SetMaterial(Material * new_tex)
{
	draw_material = new_tex; 
}

Material * UI_Image::GetMaterial() const
{
	return draw_material;
}

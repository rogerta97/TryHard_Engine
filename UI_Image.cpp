#include "UI_Image.h"
#include "OpenGL.h"
#include "Application.h"

#include "Mesh.h"
#include "Material.h"

#include "DebugDraw.h"
#include "ComponentRectTransform.h"
#include "ComponentTransform.h"
#include "UI_Plane.h"

#include "ComponentImage.h"
#include "ModuleRenderer3D.h"

UI_Image::UI_Image(ComponentImage* container)
{
	SetPercentage(0.10f); 
	CreateDrawSpace();
	cmp_container = container; 

	draw_material = (Material*)App->resources->Get(RES_MATERIAL, "blending_transparent_window");
	draw_material->LoadToMemory();
	draw_material->reference_counting++;

	image_color = { 1.0f,1.0f,1.0f };

	is_glyphy = false;
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

UI_Plane * UI_Image::GetPlane()
{
	return plane;
}

void UI_Image::Resize(float2 size)
{
	GetPlane()->Resize(size.x, size.y); 
}

void UI_Image::CleanUp()
{
	plane->CleanUp();
}

void UI_Image::CreateDrawSpace()
{
	plane = new UI_Plane();
}

void UI_Image::DrawImage()
{
	ComponentRectTransform* rtransform = (ComponentRectTransform*)cmp_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);
	ComponentTransform* trans = rtransform->GetTransform();

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

	glBindBuffer(GL_ARRAY_BUFFER, plane->GetMesh()->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (draw_material != nullptr)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, plane->GetMesh()->uvs_id);

		draw_material->GetDiffuseTexture()->Bind();

		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	}
	else
		glDisable(GL_TEXTURE_2D); 

	glColor3f(image_color.x, image_color.y, image_color.z);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane->GetMesh()->indices_id);
	glDrawElements(GL_TRIANGLES, plane->GetMesh()->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);

	if (trans)
	{
		trans->DrawAxis();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)view_mat.v);
	}
}

void UI_Image::SetImgID(uint new_id)
{
	img_textureID = new_id; 
}

uint UI_Image::GetImgID() const
{
	return img_textureID;
}

void UI_Image::SetMaterial(Material * new_tex)
{
	draw_material = new_tex; 
}

Material * UI_Image::GetMaterial() const
{
	return draw_material;
}

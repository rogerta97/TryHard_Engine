#include "UI_Button.h"
#include "ComponentButton.h"
#include "UI_Plane.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "OpenGL.h"
#include "Mesh.h"

UI_Button::UI_Button(ComponentButton* container)
{
	component_container = container; 
	clickable_area = new UI_Plane(); 
}


UI_Button::~UI_Button()
{
}

void UI_Button::Start()
{
}

void UI_Button::Update()
{
}

void UI_Button::CleanUp()
{
	clickable_area->CleanUp(); 
}

void UI_Button::Draw(bool is_editor)
{
	/*ComponentRectTransform* rtransform = (ComponentRectTransform*)cmp_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);
	ComponentTransform* trans = rtransform->GetTransform();

	App->renderer3D->UseUIRenderSettings();

	float4x4 view_mat = float4x4::identity;

	if (trans)
	{
		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		view_mat.Set((float*)matrix);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)((trans->GetGlobalViewMatrix()).Transposed() * view_mat).v);
	}*/

	if (is_editor) return; 

	App->renderer3D->UseUIRenderSettings();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, clickable_area->GetMesh()->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clickable_area->GetMesh()->indices_id);
	glDrawElements(GL_TRIANGLES, clickable_area->GetMesh()->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//if (draw_material)
	//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableClientState(GL_VERTEX_ARRAY);

	//if (trans)
	//{
	//	trans->DrawAxis();
	//	glMatrixMode(GL_MODELVIEW);
	//	glLoadMatrixf((GLfloat*)view_mat.v);
	//}
}

#include "UI_Button.h"
#include "ComponentButton.h"
#include "UI_Plane.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "OpenGL.h"
#include "Mesh.h"

#include "ComponentRectTransform.h"

UI_Button::UI_Button(ComponentButton* container)
{
	component_container = container; 
	clickable_area = new UI_Plane(); 

	transition_type = TRANSITION_ANY;
	button_state = BUTTON_IDLE; 
}


UI_Button::~UI_Button()
{
}

void UI_Button::Start()
{
}

void UI_Button::Update()
{
	// Force state

	if (GetState() == ELM_PRESSED)
	{
		SetState(ELM_IDLE);
	}

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
	{
		SetState(ELM_PRESSED); 
	}

	if (GetState() == ELM_PRESSED)
	{
		CONSOLE_LOG("Impressed"); 

		if(!component_container->OnMousePressed.empty())
			for (auto it = component_container->OnMousePressed.begin(); it != component_container->OnMousePressed.end(); it++)
			{
				std::function<void()> curr_func = (*it); 
				curr_func(); 
			}		
	}
}

void UI_Button::CleanUp()
{
	clickable_area->CleanUp(); 
}

void UI_Button::Draw(bool is_editor)
{

	if (!is_editor) return; 

	ComponentRectTransform* rtransform = (ComponentRectTransform*)component_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);
	ComponentTransform* trans = rtransform->GetTransform();

	App->renderer3D->UseImGuiRenderSettings();

	float4x4 view_mat = float4x4::identity;

	if (trans)
	{
		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		view_mat.Set((float*)matrix);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)((trans->GetGlobalViewMatrix()).Transposed() * view_mat).v);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, clickable_area->GetMesh()->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glColor3f(255.0f, 0.0f, 0.0f); 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clickable_area->GetMesh()->indices_id);
	glDrawElements(GL_TRIANGLES, clickable_area->GetMesh()->num_indices, GL_UNSIGNED_INT, NULL);

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

UI_Plane * UI_Button::GetArea() const
{
	return clickable_area;
}

Button_Transition UI_Button::GetTransition() const
{
	return transition_type;
}

void UI_Button::SetTransition(Button_Transition new_transition)
{
	transition_type = new_transition; 
}


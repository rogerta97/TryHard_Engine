#include "ComponentTextInput.h"
#include "ComponentRectTransform.h"
#include "ComponentText.h"
#include "GameObject.h"
#include "UI_Label.h"
#include "UI_TextInput.h"
#include "UI_Plane.h"
#include "UI_Button.h"
#include "Mesh.h"
#include "ComponentText.h"
#include "DebugDraw.h"
#include "ComponentButton.h"
#include "Application.h"
#include "UI_TextInput.h"
#include <string>

ComponentTextInput::ComponentTextInput(GameObject* parent)
{
	gameobject = parent; 
	component_type = CMP_TEXTINPUT; 
	input_field = new UI_TextInput(this); 

	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM); 
	GetButtonField()->GetButton()->GetArea()->Resize(rtransform->width, rtransform->height);

	cursor_mesh = new Mesh();
	cursor_mesh->SetVertPlaneData(); 
	cursor_mesh->LoadToMemory(); 

	cursor_color = { 0,0,0 };
	draw_cursor = false; 
	cursor_pos = 0; 
}


ComponentTextInput::~ComponentTextInput()
{
}

bool ComponentTextInput::Start()
{
	return false;
}

bool ComponentTextInput::Update()
{
	//The only way to click elements for now -------------

	if (App->GetGameState() != RUNNING)
		return false; 

	// ---------------------------------------------------

	if (GetButtonField()->GetButton()->GetState() == ELM_PRESSED)
	{
		GetInputField()->SetSelected(true); 
		draw_cursor = true; 

		GetButtonField()->GetButton()->SetState(ELM_IDLE);
	}


	if (draw_cursor)
	{
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
			CursorForward();

		else if (draw_cursor && App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
			CursorBackwards();

		else if (!App->user_interface->GetInputLastFrame().empty())
		{
			for (auto it = App->user_interface->GetInputLastFrame().begin(); it != App->user_interface->GetInputLastFrame().end(); it++)
			{
				if ((*it) == '\0')
					continue; 

				if ((*it) == '\x1')
				{
					DeleteTextOnCursorPos();
					break; 
				}
				
				AddTextOnCursorPos((*it));
			
			}
				
		}
	} 

	

	
	return false;
}

bool ComponentTextInput::CleanUp()
{
	return false;
}

void ComponentTextInput::AddTextOnCursorPos(const char& new_letter)
{
	ComponentRectTransform* cmp_txt_rtransform = (ComponentRectTransform*)input_field->GetShowText()->GetComponent(CMP_RECTTRANSFORM);
	ComponentText* cmp_txt = (ComponentText*)input_field->GetShowText()->GetComponent(CMP_TEXT);

	if (cmp_txt->GetContainerPlaneSize().x > cmp_txt_rtransform->width - 10)
		return; 
	
	std::string curr_text = cmp_txt->GetLabel()->GetText(); 

	std::string new_string = curr_text.substr(0, cursor_pos); 
	new_string += new_letter; 
	new_string = strcat((char*)new_string.c_str(), (char*)curr_text.substr(cursor_pos, curr_text.size() - cursor_pos).c_str());

	cmp_txt->GetLabel()->SetText(new_string.c_str());
	cursor_pos++;

}

void ComponentTextInput::DeleteTextOnCursorPos()
{	
	ComponentText* cmp_txt = (ComponentText*)input_field->GetShowText()->GetComponent(CMP_TEXT);
	std::string curr_text = cmp_txt->GetLabel()->GetText();

	if (curr_text == "" || cursor_pos == 0)
		return; 

	std::string new_string = curr_text.substr(0, cursor_pos - 1);
	new_string = strcat((char*)new_string.c_str(), (char*)curr_text.substr(cursor_pos, curr_text.size() - cursor_pos).c_str());

	cmp_txt->GetLabel()->SetText(new_string.c_str());

	cursor_pos--;

}

void ComponentTextInput::DrawButtonFrame()
{
	App->renderer3D->UseDebugRenderSettings();

	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
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

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[0].x, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[0].y, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[0].z);
	glVertex3f(GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[1].x, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[1].y, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[1].z);
																				   																	 
	glVertex3f(GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[0].x, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[0].y, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[0].z);
	glVertex3f(GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[2].x, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[2].y, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[2].z);
																				  																	  
	glVertex3f(GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[2].x, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[2].y, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[2].z);
	glVertex3f(GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[3].x, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[3].y, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[3].z);
																																					  
	glVertex3f(GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[3].x, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[3].y, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[3].z);
	glVertex3f(GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[1].x, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[1].y, GetButtonField()->GetButton()->GetArea()->GetMesh()->vertices[1].z);

	glEnd();


	if (trans)
	{
		trans->DrawAxis();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)view_mat.v);
	}
}

void ComponentTextInput::CursorForward()
{
	ComponentText* show_text_cmp = (ComponentText*)input_field->GetShowText()->GetComponent(CMP_TEXT);
	int text_lenght = show_text_cmp->GetLabel()->GetText().size(); 

	if (cursor_pos == text_lenght)
		return;
	else
		cursor_pos++;		
}

void ComponentTextInput::CursorBackwards()
{

	if (cursor_pos == 0)
		return;
	else
		cursor_pos--;
}

void ComponentTextInput::SetDrawCursor(const bool & newValue)
{
	draw_cursor = newValue;
}

void ComponentTextInput::Draw(bool is_editor)
{
	//Draw the button inside ComponentText for debugging	
	//DrawButtonFrame();
	if(GetInputField()->GetSelected())
		DrawCursor(); 	
}

void ComponentTextInput::Load(JSON_Object * json_obj)
{
	placeholder_uid = json_object_dotget_number(json_obj, "PlaceHolderID"); 
	showtext_uid = json_object_dotget_number(json_obj, "ShowTextID");
}

void ComponentTextInput::Save(JSON_Object * json_obj, const char * root)
{
	std::string item_name = root + std::string(".ComponentTextInput");

	if(input_field->GetPlaceHolderText() != nullptr)
		json_object_dotset_number(json_obj, std::string(item_name + ".PlaceHolderID").c_str(), input_field->GetPlaceHolderText()->unique_id);
	else
		json_object_dotset_number(json_obj, std::string(item_name + ".PlaceHolderID").c_str(), 0);

	if (input_field->GetShowText() != nullptr)
		json_object_dotset_number(json_obj, std::string(item_name + ".ShowTextID").c_str(), input_field->GetShowText()->unique_id);
	else
		json_object_dotset_number(json_obj, std::string(item_name + ".ShowTextID").c_str(), 0);
}

void ComponentTextInput::UpdateCursorSize()
{
	// The cursor will have the same height than the container box
	float height = 0, width = 0;
	ComponentText* show_txt_cmp = (ComponentText*)input_field->GetShowText()->GetComponent(CMP_TEXT);

	height = show_txt_cmp->GetContainerPlaneSize().y;
	width = 1; 

	cursor_mesh->vertices[0] = { -width / 2, height / 2 , 0};
	cursor_mesh->vertices[1] = { width / 2, height / 2 , 0 };
	cursor_mesh->vertices[2] = { -width / 2, -height / 2 , 0 };
	cursor_mesh->vertices[3] = { width / 2, -height / 2 , 0 };

	glBindBuffer(GL_ARRAY_BUFFER, cursor_mesh->vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * 4, cursor_mesh->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ComponentTextInput::DrawCursor()
{
	if (!draw_cursor)
		return; 

	// Get Cursor Pos
	ComponentText* text_cmp = (ComponentText*)input_field->GetShowText()->GetComponent(CMP_TEXT);
	float3 cursor_world_pos = text_cmp->GetCursorPosFromLetter(cursor_pos);

	//Generate Matrix
	float4x4 global_cursor_mat = float4x4::identity;
	global_cursor_mat.SetTranslatePart(cursor_world_pos);

	float4x4 view_mat = float4x4::identity;

	GLfloat matrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
	view_mat.Set((float*)matrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)((global_cursor_mat).Transposed() * view_mat).v);
	
	//Print Cursor
	App->renderer3D->UseDebugRenderSettings(); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, cursor_mesh->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glColor3f(cursor_color.x, cursor_color.y, cursor_color.z);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cursor_mesh->indices_id);
	glDrawElements(GL_TRIANGLES, cursor_mesh->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((GLfloat*)view_mat.v);	
}

void ComponentTextInput::OnEvent(const Event & new_event)
{
	ComponentText* cmp_txt_show = nullptr; 

	if(GetInputField()->GetShowText())
		cmp_txt_show = (ComponentText*)GetInputField()->GetShowText()->GetComponent(CMP_TEXT);

	switch (new_event.type)
	{
	case EventType::PLAY:

		break;

	case EventType::STOP:

		//cmp_txt_show->GetLabel()->CleanText();
		GetInputField()->GetShowText()->SetActive(false);
		GetInputField()->GetPlaceHolderText()->SetActive(true);

		break;

	case EventType::UI_ELEMENT_DOWN:

		//cmp_txt_show->GetLabel()->CleanText();
		if (new_event.button.but == GetButtonField()->GetButton())
		{
			if (App->GetGameState() == RUNNING)
			{
				GetButtonField()->GetButton()->SetState(ELM_PRESSED);
				GetInputField()->GetPlaceHolderText()->SetActive(false);

				GetInputField()->GetShowText()->SetActive(true);

				App->camera->SetLocked(true); 
			}
	
		}
			

		break;
	}
}

UI_TextInput * ComponentTextInput::GetInputField() const
{
	return input_field;
}

ComponentButton * ComponentTextInput::GetButtonField() const
{
	return (ComponentButton*)gameobject->GetComponent(CMP_BUTTON);
}

UID ComponentTextInput::GetPlaceHolderUID() const
{
	return placeholder_uid;
}

UID ComponentTextInput::GetShowTextUID() const
{
	return showtext_uid;
}

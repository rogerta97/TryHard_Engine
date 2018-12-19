#include "ComponentTextInput.h"
#include "ComponentRectTransform.h"
#include "GameObject.h"
#include "UI_Label.h"
#include "UI_Plane.h"
#include "UI_Button.h"
#include "ComponentText.h"
#include "DebugDraw.h"
#include "ComponentButton.h"
#include "Application.h"
#include "UI_TextInput.h"

ComponentTextInput::ComponentTextInput(GameObject* parent)
{
	gameobject = parent; 
	component_type = CMP_TEXTINPUT; 
	input_field = new UI_TextInput(this); 
	input_button = new ComponentButton(nullptr); 

	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM); 
	input_button->GetButton()->GetArea()->Resize(rtransform->width, rtransform->height);
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

	if (App->GetGameState() == RUNNING)
	{
		if (App->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
			GetButtonField()->GetButton()->SetState(ELM_PRESSED);
	}

	// ---------------------------------------------------

	if (GetButtonField()->GetButton()->GetState() == ELM_PRESSED)
	{
		CONSOLE_LOG("BUTTON HAS BEEN PRESSED"); 

		GetButtonField()->GetButton()->SetState(ELM_IDLE);
	}


	
	return false;
}

bool ComponentTextInput::CleanUp()
{
	return false;
}

void ComponentTextInput::Draw(bool is_editor)
{

	//Draw the button inside ComponentText for debugging
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

	glVertex3f(input_button->GetButton()->GetArea()->GetMesh()->vertices[0].x, input_button->GetButton()->GetArea()->GetMesh()->vertices[0].y, input_button->GetButton()->GetArea()->GetMesh()->vertices[0].z);
	glVertex3f(input_button->GetButton()->GetArea()->GetMesh()->vertices[1].x, input_button->GetButton()->GetArea()->GetMesh()->vertices[1].y, input_button->GetButton()->GetArea()->GetMesh()->vertices[1].z);

	glVertex3f(input_button->GetButton()->GetArea()->GetMesh()->vertices[0].x, input_button->GetButton()->GetArea()->GetMesh()->vertices[0].y, input_button->GetButton()->GetArea()->GetMesh()->vertices[0].z);
	glVertex3f(input_button->GetButton()->GetArea()->GetMesh()->vertices[2].x, input_button->GetButton()->GetArea()->GetMesh()->vertices[2].y, input_button->GetButton()->GetArea()->GetMesh()->vertices[2].z);

	glVertex3f(input_button->GetButton()->GetArea()->GetMesh()->vertices[2].x, input_button->GetButton()->GetArea()->GetMesh()->vertices[2].y, input_button->GetButton()->GetArea()->GetMesh()->vertices[2].z);
	glVertex3f(input_button->GetButton()->GetArea()->GetMesh()->vertices[3].x, input_button->GetButton()->GetArea()->GetMesh()->vertices[3].y, input_button->GetButton()->GetArea()->GetMesh()->vertices[3].z);

	glVertex3f(input_button->GetButton()->GetArea()->GetMesh()->vertices[3].x, input_button->GetButton()->GetArea()->GetMesh()->vertices[3].y, input_button->GetButton()->GetArea()->GetMesh()->vertices[3].z);
	glVertex3f(input_button->GetButton()->GetArea()->GetMesh()->vertices[1].x, input_button->GetButton()->GetArea()->GetMesh()->vertices[1].y, input_button->GetButton()->GetArea()->GetMesh()->vertices[1].z);

	glEnd();


	if (trans)
	{
		trans->DrawAxis();
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)view_mat.v);
	}

	
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

void ComponentTextInput::OnEvent(const Event & new_event)
{
	ComponentText* cmp_txt_show = nullptr; 

	if(GetInputField()->GetShowText())
		cmp_txt_show = (ComponentText*)GetInputField()->GetShowText()->GetComponent(CMP_TEXT);

	switch (new_event.type)
	{
	case EventType::PLAY:

		cmp_txt_show->GetLabel()->CleanText();
		GetInputField()->GetShowText()->SetActive(true);

		break;

	case EventType::STOP:

		cmp_txt_show->GetLabel()->CleanText();
		GetInputField()->GetShowText()->SetActive(false);
		GetInputField()->GetPlaceHolderText()->SetActive(true);

		break;
	}
}

UI_TextInput * ComponentTextInput::GetInputField() const
{
	return input_field;
}

ComponentButton * ComponentTextInput::GetButtonField() const
{
	return input_button;
}

UID ComponentTextInput::GetPlaceHolderUID() const
{
	return placeholder_uid;
}

UID ComponentTextInput::GetShowTextUID() const
{
	return showtext_uid;
}

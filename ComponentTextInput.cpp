#include "ComponentTextInput.h"
#include "GameObject.h"
#include "UI_Label.h"
#include "ComponentText.h"
#include "Application.h"
#include "UI_TextInput.h"

ComponentTextInput::ComponentTextInput(GameObject* parent)
{
	gameobject = parent; 
	component_type = CMP_TEXTINPUT; 
	input_field = new UI_TextInput(this); 
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
	return false;
}

bool ComponentTextInput::CleanUp()
{
	return false;
}

void ComponentTextInput::Draw(bool is_editor)
{
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

UID ComponentTextInput::GetPlaceHolderUID() const
{
	return placeholder_uid;
}

UID ComponentTextInput::GetShowTextUID() const
{
	return showtext_uid;
}

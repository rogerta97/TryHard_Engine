#include "ComponentTextInput.h"
#include "GameObject.h"
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

void ComponentTextInput::OnEvent(const Event & new_event)
{
	switch (new_event.type)
	{
	case EventType::PLAY:

		CONSOLE_LOG("IM THE BUTTON AND RECEIVED THE EVENT"); 

		break;

	case EventType::PAUSE:

		//GetInputField()->GetPlaceHolderText()->SetActive(true); 
		//GetInputField()->GetShowText()->SetActive(false);

		break;

	}
}

UI_TextInput * ComponentTextInput::GetInputField() const
{
	return input_field;
}

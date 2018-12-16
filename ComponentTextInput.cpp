#include "ComponentTextInput.h"
#include "GameObject.h"
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

UI_TextInput * ComponentTextInput::GetInputField() const
{
	return input_field;
}

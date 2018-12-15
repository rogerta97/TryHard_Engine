#include "ComponentTextInput.h"
#include "GameObject.h"


ComponentTextInput::ComponentTextInput(GameObject* parent)
{
	gameobject = parent; 
	component_type = CMP_TEXTINPUT; 
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

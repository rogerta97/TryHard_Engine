#include "ComponentText.h"
#include "UI_Label.h"


ComponentText::ComponentText(GameObject* parent)
{
	component_type = CMP_TEXT;
	gameobject = parent;
	label = new UI_Label(this); 
}


ComponentText::~ComponentText()
{
}

bool ComponentText::Start()
{
	return false;
}

bool ComponentText::Update()
{
	return false;
}

bool ComponentText::CleanUp()
{
	return false;
}

void ComponentText::Draw(bool is_editor)
{
}

UI_Label * ComponentText::GetLabel() const
{
	return label;
}

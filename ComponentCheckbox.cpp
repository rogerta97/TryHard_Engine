#include "ComponentCheckbox.h"



ComponentCheckbox::ComponentCheckbox()
{
}


ComponentCheckbox::~ComponentCheckbox()
{
}

bool ComponentCheckbox::Start()
{
	return false;
}

bool ComponentCheckbox::Update()
{
	return false;
}

bool ComponentCheckbox::CleanUp()
{
	return false;
}

void ComponentCheckbox::Draw(bool is_editor)
{
}

void ComponentCheckbox::FitToRect()
{
}

void ComponentCheckbox::OnEvent(const Event & new_event)
{
}

void ComponentCheckbox::Load(JSON_Object * json_obj)
{
}

void ComponentCheckbox::Save(JSON_Object * json_obj, const char * root)
{
}

UI_CheckBox * ComponentCheckbox::GetCheckBox() const
{
	return checkbox;
}

void ComponentCheckbox::BindCallbackFunctions()
{
}

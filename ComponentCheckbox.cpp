#include "ComponentCheckBox.h"
#include "UI_CheckBox.h"
#include "GameObject.h"
#include "UICallbackSystem.h"
ComponentCheckBox::ComponentCheckBox(GameObject* parent)
{
	gameobject = parent; 
	component_type = CMP_CHECKBOX; 
	checkbox = new UI_CheckBox(this); 

	callback_system = new UICallbackSystem(this); 

	background_img_pos_percentage = 0.2f;
	label_origin_pos_percentage = 0.35f;
}

ComponentCheckBox::~ComponentCheckBox()
{
}

bool ComponentCheckBox::Start()
{
	return false;
}

bool ComponentCheckBox::Update()
{
	return false;
}

bool ComponentCheckBox::CleanUp()
{
	return false;
}

void ComponentCheckBox::Draw(bool is_editor)
{
}

void ComponentCheckBox::FitToRect()
{
}

void ComponentCheckBox::OnEvent(const Event & new_event)
{
}

void ComponentCheckBox::Load(JSON_Object * json_obj)
{
}

void ComponentCheckBox::Save(JSON_Object * json_obj, const char * root)
{
}

float ComponentCheckBox::GetBackgroundDistancePercentage()
{
	return background_img_pos_percentage;
}

float ComponentCheckBox::GetLabelPercentage()
{
	return label_origin_pos_percentage;
}

UI_CheckBox * ComponentCheckBox::GetCheckBox() const
{
	return checkbox;
}

void ComponentCheckBox::BindCallbackFunctions()
{

}

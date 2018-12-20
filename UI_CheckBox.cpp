#include "UI_CheckBox.h"



UI_CheckBox::UI_CheckBox(ComponentCheckBox* parent_cmp)
{
	cmp_container = parent_cmp; 
	SetPercentage(0.3f); 
	checkbox_type = CHT_TOGGLE_BOOL; 
}


UI_CheckBox::~UI_CheckBox()
{
}

void UI_CheckBox::Start()
{
}

void UI_CheckBox::Update()
{
}

void UI_CheckBox::CleanUp()
{
}

void UI_CheckBox::Draw()
{
}

bool UI_CheckBox::GetIsOn() const
{
	return is_on;
}

void UI_CheckBox::SetIsOn(const bool & is_on)
{
	this->is_on = is_on; 
}

CheckBoxType UI_CheckBox::GetType() const
{
	return checkbox_type;
}

void UI_CheckBox::SetType(const CheckBoxType & is_on)
{
	checkbox_type = is_on; 
}

#include "UI_TextInput.h"

#include "UI_Label.h"
#include "UI_Image.h"


UI_TextInput::UI_TextInput(ComponentTextInput* parent_cmp)
{
	placeholder_text = nullptr; 
	show_text = nullptr; 
	SetPercentage(0.30f); 
	this->parent_cmp = parent_cmp; 
}


UI_TextInput::~UI_TextInput()
{
}

void UI_TextInput::SetPlaceHolderText(GameObject * new_placeholder)
{
	placeholder_text = new_placeholder;
}

GameObject * UI_TextInput::GetPlaceHolderText() const
{
	return placeholder_text;
}

void UI_TextInput::SetShowText(GameObject * new_show)
{
	show_text = new_show; 
}

GameObject * UI_TextInput::GetShowText() const
{
	return show_text;
}

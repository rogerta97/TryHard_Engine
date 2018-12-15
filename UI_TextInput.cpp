#include "UI_TextInput.h"

#include "UI_Label.h"
#include "UI_Image.h"


UI_TextInput::UI_TextInput()
{
	placeholder_text = nullptr; 
	show_text = nullptr; 

	//background_img = new UI_Image();
}


UI_TextInput::~UI_TextInput()
{
}

void UI_TextInput::SetPlaceHolderText(UI_Label * new_placeholder)
{
	placeholder_text = new_placeholder;
}

UI_Label * UI_TextInput::GetPlaceHolderText() const
{
	return placeholder_text;
}

void UI_TextInput::SetShowText(UI_Label * new_show)
{
	show_text = new_show; 
}

UI_Label * UI_TextInput::SetShowText() const
{
	return show_text;
}

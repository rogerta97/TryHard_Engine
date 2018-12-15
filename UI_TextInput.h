#pragma once
#include "UI_Element.h"

class UI_Label; 
class UI_Image; 

class UI_TextInput : public UI_Element
{
public:
	UI_TextInput();
	~UI_TextInput();

	void SetPlaceHolderText(UI_Label* new_placeholder);
	UI_Label* GetPlaceHolderText() const;

	void SetShowText(UI_Label* new_placeholder);
	UI_Label* SetShowText() const;

private:

	UI_Label * placeholder_text; 
	UI_Label * show_text; 
};


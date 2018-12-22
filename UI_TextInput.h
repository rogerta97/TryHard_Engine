#pragma once
#include "UI_Element.h"

class UI_Label; 
class UI_Image; 
class GameObject; 
class ComponentTextInput; 

class UI_TextInput : public UI_Element
{
public:
	UI_TextInput(ComponentTextInput* parent_cmp);
	~UI_TextInput();

	ComponentTextInput* parent_cmp = nullptr;

	void SetPlaceHolderText(GameObject* new_placeholder);
	GameObject* GetPlaceHolderText() const;

	void SetShowText(GameObject* new_placeholder);
	GameObject* GetShowText() const;

private:

	GameObject * placeholder_text = nullptr; 
	GameObject * show_text = nullptr; 
};


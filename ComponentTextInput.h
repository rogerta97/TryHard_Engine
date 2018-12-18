#pragma once
#include "Component.h"

class GameObject; 
class UI_TextInput; 

class ComponentTextInput : public Component
{
public:
	ComponentTextInput(GameObject* parent);
	~ComponentTextInput();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor);

	void OnEvent(const Event& new_event);

	UI_TextInput* GetInputField() const;

private:

	UI_TextInput * input_field; 
};


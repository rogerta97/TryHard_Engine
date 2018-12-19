#pragma once
#include "Component.h"
#include "Globals.h"

class GameObject; 
class UI_TextInput; 
class ComponentButton; 

class ComponentTextInput : public Component
{
public:
	ComponentTextInput(GameObject* parent);
	~ComponentTextInput();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor);

	void Load(JSON_Object* json_obj);
	void Save(JSON_Object* json_obj, const char* root);

	void OnEvent(const Event& new_event);

	UI_TextInput* GetInputField() const;
	ComponentButton* GetButtonField() const;

	UID GetPlaceHolderUID() const;
	UID GetShowTextUID() const;

private:

	UID placeholder_uid; 
	UID showtext_uid; 

	UI_TextInput * input_field; 
	ComponentButton * input_button; 
};


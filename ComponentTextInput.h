#pragma once
#include "Component.h"
#include "Globals.h"

class GameObject; 
class UI_TextInput; 
class Mesh;
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

	void UpdateCursorSize(); 
	void DrawCursor();
	void DrawButtonFrame();

	void SetDrawCursor(const bool& newValue); 

	void OnEvent(const Event& new_event);

	UI_TextInput* GetInputField() const;
	ComponentButton* GetButtonField() const;

	UID GetPlaceHolderUID() const;
	UID GetShowTextUID() const;

private:
	UID placeholder_uid = 0; 
	UID showtext_uid = 0; 

	bool draw_cursor = false; 
	uint cursor_pos = 0; 

	UI_TextInput * input_field = nullptr; 
	ComponentButton * input_button = nullptr; 
	Mesh* cursor_mesh = nullptr;
	float3 cursor_color = { 0,0,0 }; 
};


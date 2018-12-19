#ifndef  _COMPONENT_CHECKBOX_H_
#define _COMPONENT_CHECKBOX_H_

#include "Component.h"
#include <functional>

class UI_CheckBox;
class UICallbackSystem;

class GameObject; 

class ComponentCheckBox :public Component
{
public:
	ComponentCheckBox(GameObject* parent);
	~ComponentCheckBox();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor);
	void FitToRect();
	void OnEvent(const Event& new_event);

	void Load(JSON_Object* json_obj);
	void Save(JSON_Object* json_obj, const char* root);

	float GetBackgroundDistancePercentage(); 
	float GetLabelPercentage();

	UI_CheckBox* GetCheckBox() const;

	UICallbackSystem* callback_system;

	void BindCallbackFunctions();

	std::list<std::function<void(bool)>> OnCheckBoxPressed; // CheckBox will only accept true/false functions

private:

	float background_img_pos_percentage = 0.0f; 
	float label_origin_pos_percentage = 0.0f;

	UI_CheckBox* checkbox = nullptr;
};

#endif // ! _COMPONENT_CHECKBOX_H_




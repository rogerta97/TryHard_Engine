#ifndef  _COMPONENT_CHECKBOX_H_
#define _COMPONENT_CHECKBOX_H_

#include "Component.h"
#include <functional>
#include <vector>

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

	float2 GetBackgroundDistancePercentage(); 
	float2 GetLabelPercentage();

	UI_CheckBox* GetCheckBox() const;

	UICallbackSystem* callback_system;

	void BindCallbackFunctions();

	std::vector<std::function<void()>> ButtonOnAction;
	std::vector<std::function<void()>> ButtonOffAction;

	void DoActions();

private:

	bool toggle_done = false; 

	float2 background_img_pos_percentage = float2::zero;
	float2 label_origin_pos_percentage = float2::zero;

	UI_CheckBox* checkbox = nullptr;
};

#endif // ! _COMPONENT_CHECKBOX_H_




#ifndef  _COMPONENT_CHECKBOX_H_
#define _COMPONENT_CHECKBOX_H_

#include "Component.h"
#include <functional>

class UI_CheckBox;
class UICallbackSystem;

class ComponentCheckbox :public Component
{
public:
	ComponentCheckbox();
	~ComponentCheckbox();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor);
	void FitToRect();
	void OnEvent(const Event& new_event);

	void Load(JSON_Object* json_obj);
	void Save(JSON_Object* json_obj, const char* root);

	UI_CheckBox* GetCheckBox() const;

	UICallbackSystem* callback_system;

	void BindCallbackFunctions();

	std::list<std::function<void()>> OnMousePressed;

private:

	UI_CheckBox* checkbox;
};

#endif // ! _COMPONENT_CHECKBOX_H_




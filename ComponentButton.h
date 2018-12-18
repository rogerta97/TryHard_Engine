#pragma once
#include "Component.h"

#include <functional>
#include <list>

class UI_Button;
class UICallbackSystem; 

class ComponentButton : public Component
{
public:
	ComponentButton(GameObject* parent);
	~ComponentButton();

	bool Start();
	bool Update();
	bool CleanUp();
	void Draw(bool is_editor);
	void FitToRect(); 

	void Load(JSON_Object* json_obj);
	void Save(JSON_Object* json_obj, const char* root);

	UI_Button* GetButton() const;

	UICallbackSystem* callback_system;

	void BindCallbackFunctions(); 

	std::list<std::function<void()>> OnMousePressed; 

private:
	UI_Button * button;
};


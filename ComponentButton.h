#ifndef _COMPONENT_BUTTON_H_
#define _COMPONENT_BUTTON_H_

#include "Component.h"

#include <functional>
#include <list>
#include <vector>

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
	void OnEvent(const Event& new_event);

	void Load(JSON_Object* json_obj);
	void Save(JSON_Object* json_obj, const char* root);

	std::vector<std::function<void()>> OnMousePressed;

	UI_Button* GetButton() const;

	// Callbacks
	UICallbackSystem* callback_system;
	void BindCallbackFunctions(); 

	// Set & Get 

	float3 GetHoverColor()const; 
	void SetHoverColor(const float3& new_color);

	float3 GetPressedColor()const;
	void SetPressedColor(const float3& new_color);

	void SetImageColor(const float3& new_color);

	bool has_mouse_entered = false; 

private:
	UI_Button * button;

	float3 hover_color = { 0,0,0 }; 
	float3 pressed_color = { 0.5,0,0 };
};

#endif


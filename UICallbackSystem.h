#ifndef _UI_CALLBACK_SYSTEM_
#define _UI_CALLBACK_SYSTEM_

#include <functional>
#include "UI_Element.h"
#include <list>

class GameObject; 
class ComponentButton; 
class ComponentCheckBox; 
class UICallbackSystem;
class Component;

struct UI_CallbackAgent
{

public: 

	UI_CallbackAgent(UICallbackSystem* container);

	void CleanAgent(); 
	void PrintAgentUI(int index);
	void SetEmpty(); 
	UI_CallbackAgent* Duplicate(); 

public:

	GameObject* parent; 
	std::string name;	
	UICallbackSystem* system_container; 

	std::function<void()> action;
	std::function<void(const char*)> action_char;
	std::function<void(bool)> action_bool;

	const char* value_char = "";
	bool value_bool = true;

	bool show_function_list; 
};

class UICallbackSystem
{
public:
	UICallbackSystem(ComponentButton* attached);
	UICallbackSystem(ComponentCheckBox* attached);
	~UICallbackSystem();
	
	void CleanSystem(); 

	void PrintSystemUI(); 
	UI_CallbackAgent* CreateEmptyAgent();

	std::list<UI_CallbackAgent*>& GetCallbacks(); 

	Component* GetSystemOwner() const; 

	void SetSystemOwnerButton(ComponentButton* new_owner);
	void SetSystemOwnerCheckBox(ComponentCheckBox* new_owner);

	bool show_function_list; 

	UI_Widgget_Type attached_to = UI_Widgget_Type::UI_NULL; 

private:

	std::list<UI_CallbackAgent*> callbacks_list;
	ComponentButton* button_cmp_attached;
	ComponentCheckBox* checkbox_cmp_attached; 

};

#endif // !_UI_CALLBACK_SYSTEM




#ifndef _UI_CALLBACK_SYSTEM_
#define _UI_CALLBACK_SYSTEM_

#include <functional>
#include <list>

class GameObject; 
class ComponentButton; 
class UICallbackSystem;


struct UI_CallbackAgent
{
	UI_CallbackAgent(UICallbackSystem* container);

	void CleanAgent(); 

	GameObject* parent; 

	std::function<void()> action;
	std::function<void(const char*)> action_char;

	std::string name;

	const char* value_char = ""; 

	UICallbackSystem* system_container; 

	void PrintAgentUI();
};

class UICallbackSystem
{
public:
	UICallbackSystem(ComponentButton* attached);
	~UICallbackSystem();
	
	void CleanSystem(); 

	void PrintSystemUI(); 
	UI_CallbackAgent* CreateEmptyAgent();

	std::list<UI_CallbackAgent*> GetCallbacks() const; 

	ComponentButton* GetSystemOwner() const; 
	void SetSystemOwner(ComponentButton* new_owner);

	bool show_function_list; 

private:

	std::list<UI_CallbackAgent*> callbacks_list;
	ComponentButton* button_cmp_attached;

};

#endif // !_UI_CALLBACK_SYSTEM




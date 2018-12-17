#ifndef _UI_CALLBACK_SYSTEM_
#define _UI_CALLBACK_SYSTEM_

#include <functional>
#include <list>

class GameObject; 
class ComponentButton; 
class UICallbackSystem;


struct UI_CallbackAgent
{

public: 

	UI_CallbackAgent(UICallbackSystem* container);

	void CleanAgent(); 
	void PrintAgentUI(int index);
	void SetEmpty(); 


public:

	GameObject* parent; 
	std::string name;	
	UICallbackSystem* system_container; 

	std::function<void()> action;

	std::function<void(const char*)> action_char;
	const char* value_char = "";

	bool show_function_list; 
};

class UICallbackSystem
{
public:
	UICallbackSystem(ComponentButton* attached);
	~UICallbackSystem();
	
	void CleanSystem(); 

	void PrintSystemUI(); 
	UI_CallbackAgent* CreateEmptyAgent();

	std::list<UI_CallbackAgent*>& GetCallbacks(); 

	ComponentButton* GetSystemOwner() const; 
	void SetSystemOwner(ComponentButton* new_owner);

	bool show_function_list; 

private:

	std::list<UI_CallbackAgent*> callbacks_list;
	ComponentButton* button_cmp_attached;

};

#endif // !_UI_CALLBACK_SYSTEM




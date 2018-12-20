#include "UICallbackSystem.h"
#include "imgui.h"
#include "GameObject.h"
#include "UICallbackSystem.h"
#include "ComponentButton.h"
#include "ComponentCheckbox.h"
#include "Application.h"

UICallbackSystem::UICallbackSystem(ComponentButton* cmp)
{
	checkbox_cmp_attached = nullptr;
	button_cmp_attached = cmp; 
	show_function_list = false; 
	attached_to = UI_BUTTON;
}

UICallbackSystem::UICallbackSystem(ComponentCheckBox* cmp)
{
	button_cmp_attached = nullptr;
	checkbox_cmp_attached = cmp;
	show_function_list = false;
	attached_to = UI_CHECKBOX; 
}


UICallbackSystem::~UICallbackSystem()
{
}

void UICallbackSystem::CleanSystem()
{
	for (auto it = callbacks_list.begin(); it != callbacks_list.end(); it++)
	{
		(*it)->CleanAgent();
		delete (*it); 
	}

	button_cmp_attached = nullptr; 
	callbacks_list.clear(); 
}

void UICallbackSystem::PrintSystemUI()
{
	ImGui::Spacing();

	if (attached_to == UI_BUTTON)
		ImGui::Text("On Click Actions: ");

	else if (attached_to == UI_CHECKBOX)
		ImGui::Text("Function To Toggle: "); 

	ImGui::SameLine(); 
	if (ImGui::SmallButton("Add Item##AddObject"))
	{
		UI_CallbackAgent* new_agent = CreateEmptyAgent();

		switch (attached_to)
		{
		case UI_Widgget_Type::UI_BUTTON:
			new_agent->system_container->button_cmp_attached->OnMousePressed.push_back(new_agent->action);
			break;

		case UI_Widgget_Type::UI_CHECKBOX:
		{
			new_agent->system_container->checkbox_cmp_attached->ButtonOnAction.push_back(new_agent->action);
			break;
		}
		
		}
		
	}
		
	ImGui::SameLine();
	ImGui::SmallButton("Delete Item");

	SEPARATE_WITH_SPACE

	int index = 0; 
	for (auto it = callbacks_list.begin(); it != callbacks_list.end(); it++)	
		(*it)->PrintAgentUI(index++); 
	
}

UI_CallbackAgent * UICallbackSystem::CreateEmptyAgent()
{
	UI_CallbackAgent* new_agent = new UI_CallbackAgent(this); 
	callbacks_list.push_back(new_agent); 
	new_agent->name = "";

	return new_agent;
}

std::list<UI_CallbackAgent*>& UICallbackSystem::GetCallbacks()
{
	return callbacks_list;
}

Component* UICallbackSystem::GetSystemOwner() const
{
	if(button_cmp_attached)
		return button_cmp_attached;

	else if (checkbox_cmp_attached)
		return checkbox_cmp_attached;
}

void UICallbackSystem::SetSystemOwnerButton(ComponentButton * new_owner)
{
	button_cmp_attached = new_owner; 
}

void UICallbackSystem::SetSystemOwnerCheckBox(ComponentCheckBox * new_owner)
{
	checkbox_cmp_attached = new_owner;
}

UI_CallbackAgent::UI_CallbackAgent(UICallbackSystem* system_container)
{
	parent = nullptr; 
	action = nullptr; 

	action_char = nullptr; 
	value_char = ""; 

	name = ""; 

	this->system_container = system_container; 
	show_function_list = false; 
}

void UI_CallbackAgent::CleanAgent()
{
	parent = nullptr; 
	action = nullptr; 
	system_container = nullptr; 
}

void UI_CallbackAgent::PrintAgentUI(int index)
{

	//ImGui::Text("Target: "); ImGui::SameLine();

	//if (parent)
	//	ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", parent->GetName().c_str());
	//else
	//	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Empty");

	//ImGui::SameLine();

	string curr_label_name = "";
	/* = "+ Target##Target" + to_string(index);*/
	//if (ImGui::Button(curr_label_name.c_str()))
	//{

	//}

	//ImGui::SameLine();
	//ImGui::Button("- Target##Target");


	if (ImGui::IsItemClicked(0))
	{
		parent = nullptr;
	}

	ImGui::Text("Action: "); ImGui::SameLine();

	if (action || action_char || action_bool)
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", name.c_str());
	else
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Empty");

	ImGui::SameLine();

	curr_label_name = "+ Action##Action" + to_string(index);
	ImGui::Button(curr_label_name.c_str());

	if (ImGui::IsItemClicked(0))
	{
		ImGui::OpenPopup("select_callback");
		show_function_list = true;
	}

	ImGui::SameLine();

	curr_label_name = "- Action##Action" + to_string(index);
	ImGui::Button(curr_label_name.c_str());

	if (ImGui::IsItemClicked(0))
	{
		SetEmpty(); 
	}

	string label = "";

	if (action_char != nullptr)
	{
		ImGui::SameLine();

		static std::string value_char_cpy(value_char); 

		label = "Value##S" + to_string(index);
		if (ImGui::InputText(label.c_str(), (char*)value_char_cpy.c_str(), 256))
			value_char = value_char_cpy.c_str(); 
	}

	if (action_bool != nullptr)
	{
		ImGui::SameLine();
		static bool send_value = true;
		label = "Value##C" + to_string(index);
		if (ImGui::Checkbox(label.c_str() , &send_value))
		{

		}
	}

					
	if (show_function_list)
	{
		App->script->PrintFunctionsList(this, index);
	}

	if (show_function_list && ImGui::IsMouseClicked(0) && ImGui::IsMouseHoveringWindow() && !ImGui::IsAnyItemHovered())
		show_function_list = false; 		

}

void UI_CallbackAgent::SetEmpty()
{
	parent = nullptr; 
	name = ""; 
	
	action = nullptr; 
	action_char = nullptr; 
	action_bool = nullptr; 

	value_char = ""; 
	value_bool = true; 

	show_function_list = false; 
}

UI_CallbackAgent * UI_CallbackAgent::Duplicate()
{
	UI_CallbackAgent* new_agent = new UI_CallbackAgent(system_container);

	new_agent->parent = parent; 
	new_agent->name = name;

	new_agent->action = action;
	new_agent->action_char = action_char;
	new_agent->action_bool = action_bool;

	new_agent->value_char = value_char;
	new_agent->value_bool = value_bool;

	new_agent->show_function_list = false; 

	return new_agent;
}

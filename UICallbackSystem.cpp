#include "UICallbackSystem.h"
#include "imgui.h"
#include "GameObject.h"
#include "UICallbackSystem.h"
#include "ComponentButton.h"
#include "Application.h"

UICallbackSystem::UICallbackSystem(ComponentButton* cmp)
{
	button_cmp_attached = cmp; 
	show_function_list = false; 
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
	ImGui::Text("On Click Actions"); 

	ImGui::Separator();

	for (auto it = callbacks_list.begin(); it != callbacks_list.end(); it++)	
		(*it)->PrintAgentUI(); 
	
	ImGui::Separator(); 

	if (ImGui::Button("+##AddObject"))	
		CreateEmptyAgent(); 
	
	ImGui::SameLine();
	ImGui::Button("-"); 
}

UI_CallbackAgent * UICallbackSystem::CreateEmptyAgent()
{
	UI_CallbackAgent* new_agent = new UI_CallbackAgent(this); 
	callbacks_list.push_back(new_agent); 
	new_agent->name = "";

	return new_agent;
}

ComponentButton * UICallbackSystem::GetSystemOwner() const
{
	return button_cmp_attached;
}

void UICallbackSystem::SetSystemOwner(ComponentButton * new_owner)
{
	button_cmp_attached = new_owner; 
}

UI_CallbackAgent::UI_CallbackAgent(UICallbackSystem* system_container)
{
	parent = nullptr; 
	action = nullptr; 
	name = ""; 

	this->system_container = system_container; 
}

void UI_CallbackAgent::CleanAgent()
{
	parent = nullptr; 
	action = nullptr; 
	system_container = nullptr; 
}

void UI_CallbackAgent::PrintAgentUI()
{
	ImGui::Separator();

	ImGui::Text("Target: "); ImGui::SameLine();

	if (parent)
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", parent->GetName().c_str());
	else
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Empty"); 

	ImGui::SameLine(); 
	if (ImGui::Button("+##Target"))
	{
		
	}

	ImGui::SameLine();
	ImGui::Button("-##Target");

	if(ImGui::IsItemClicked(0))
	{
		parent = nullptr; 
	}

	//Add GameObject Button

	ImGui::Text("Action: "); ImGui::SameLine();

	if (action)
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", name.c_str());
	else
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Empty");

	ImGui::SameLine();
	ImGui::Button("+##Action");

	if (ImGui::IsItemClicked(0))
	{
		ImGui::OpenPopup("select_callback");
		system_container->show_function_list = true;
	}

	ImGui::SameLine();
	ImGui::Button("-##Action");
	
	if (ImGui::IsItemClicked(0))
	{
		action = nullptr;
		name = "";
	}		
			
	std::pair<const char*, std::function<void()>> func("", nullptr); 
	
	if(system_container->show_function_list)
		func = App->script->PrintFunctionList();

	if (func.first != "" && func.second != nullptr)
	{
		ComponentButton* button_cmp = system_container->GetSystemOwner();
		
		action = func.second; 
		name = func.first; 

		button_cmp->OnMousePressed = action;
	}
		
	ImGui::Separator();

}

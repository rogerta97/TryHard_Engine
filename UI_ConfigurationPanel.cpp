#include "UI_ConfigurationPanel.h"
#include "Application.h"

UI_ConfigurationPanel::UI_ConfigurationPanel()
{
}

UI_ConfigurationPanel::~UI_ConfigurationPanel()
{
}

bool UI_ConfigurationPanel::Start()
{
	show = false; 

	return true;
}

bool UI_ConfigurationPanel::Update()
{
	if (show == false)
		return false; 

	ImGui::Begin("Configuration", &show); 

	if(ImGui::CollapsingHeader("Camera"))
	{
		float tmp_speed = App->camera->GetSpeed();
		ImGui::SliderFloat("Speed", &tmp_speed, 0.1f, 20.0f, "%.2f");
		App->camera->SetSpeed(tmp_speed); 
	}

	ImGui::End(); 

	return true;
}

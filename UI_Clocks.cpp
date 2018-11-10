#include "UI_ClocksPanel.h"
#include "ModuleTime.h"
#include "Application.h"


UI_ClocksPanel::UI_ClocksPanel()
{
	name = "ScenePanel"; 
}


UI_ClocksPanel::~UI_ClocksPanel()
{
}

bool UI_ClocksPanel::Start()
{
	return true;
}

bool UI_ClocksPanel::Update()
{
	ImGuiWindowFlags flags = NULL;

	float real_time, game_time;

	Timer& game_timer = App->time_manager->game_timer;

	Timer& real_timer = App->time_manager->real_timer;
	
	real_time = real_timer.Read();

	game_time = game_timer.Read();

	//Transform to seconds
	real_time /= 1000;
	game_time /= 1000;

	if (ImGui::Begin("Clocks", &show, flags))
	{
		ImGui::Text("Real time %.2f", real_time);

		ImGui::Text("Game time %.2f", game_time);
	}

	ImGui::End();
	return true;
}

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
	show = false; 

	return true;
}

bool UI_ClocksPanel::Update()
{
	ImGuiWindowFlags flags = NULL;

	float real_time, game_time;

	Timer& game_timer = App->time_manager->game_timer;

	Timer& real_timer = App->time_manager->real_timer;
	
	real_time = real_timer.Read();

	game_time = App->time_manager->game_time;

	//Transform to seconds
	real_time /= 1000;
	game_time /= 1000;

	if (ImGui::Begin("Clocks", &show, flags))
	{
		ImGui::Text("Real time %.2f s", real_time);

		ImGui::Text("Game time %.2f s",game_time);

		ImGui::Text("Game dt %.2f ms", App->time_manager->game_dt);

		ImGui::Text("Frame count %d", App->time_manager->frame_count);

		ImGui::DragFloat("Time Scale", &App->time_manager->time_scale, 0.01f, 0.1f, 3.0f,"%.2f");
	}

	ImGui::End();
	return true;
}

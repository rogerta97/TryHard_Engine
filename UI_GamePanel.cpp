#include "UI_GamePanel.h"



UI_GamePanel::UI_GamePanel()
{
}


UI_GamePanel::~UI_GamePanel()
{
}

bool UI_GamePanel::Start()
{
	show = true; 
	return true;
}

bool UI_GamePanel::Update()
{
	if (ImGui::Begin("Game", &show, NULL))
	{
	

		ImGui::End();
	}

	return true;
}

bool UI_GamePanel::CleanUp()
{
	return true;
}

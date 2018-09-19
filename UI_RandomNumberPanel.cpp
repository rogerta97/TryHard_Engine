#include "UI_RandomNumberPanel.h"
#include "Application.h"
#include "Functions.h"
#include "imgui_dock.h"

UI_RandomNumberPanel::UI_RandomNumberPanel()
{
}


UI_RandomNumberPanel::~UI_RandomNumberPanel()
{
}

bool UI_RandomNumberPanel::Start()
{
	generated_num = 0; 
	random_type = RND_TYPE_INT; 
	show_warning = false; 
	show = true; 

	return true;
}

bool UI_RandomNumberPanel::Update()
{
	if (show == false)
		return true; 

	if (ImGui::BeginDock("Random Generator", NULL, NULL))
	{	
		//ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "This engine is using simplified PCG library to generate random number creation. Select Help->Libraries to get extense information about it."); 
		ImGui::TextWrapped("This engine is using simplified PCG library to generate random number creation. Select Help->Libraries to get extense information about it.");
		int rt = (int)random_type; 
		ImGui::Combo("Number Type", &rt, "Integer\0Float");
		random_type = (RandomNumberType)rt;

		if (random_type == RND_TYPE_INT)
		{
			ImGui::InputInt("Max", &max);
			ImGui::InputInt("Min", &min);
		}
		else
		{
			ImGui::InputFloat("Max", &maxf);
			ImGui::InputFloat("Min", &minf);
		}

		if (show_warning)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Max & Min values should not match.");
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Max should be > Min.");
			ImGui::SameLine(); 

			if (ImGui::Button("OK"))
				show_warning = false; 

			ImGui::EndDock();
			return true;
		}

		if (ImGui::Button("Generate"))
		{
	
			if (random_type == RND_TYPE_INT)
			{
				if (max == min)
					show_warning = true; 
				else
					generated_num = GetPCGRandomNumber((float)max, (float)min);
			}
				
			else 
			{
				if (maxf == minf)
					show_warning = true;
				else
					generated_num = GetPCGRandomNumber((float)maxf, (float)minf);
			}
				

		} ImGui::SameLine(); 

		if (ImGui::Button("Assign"))
		{
			
		}

		ImGui::SameLine(); 

		if (random_type == RND_TYPE_INT)
		{
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Number: %d", (int)generated_num); 
		}
		else
		{
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Number: %f", generated_num);
		}

	}

	ImGui::EndDock();

	return true;
}

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
	show = false; 

	return true;
}

bool UI_RandomNumberPanel::Update()
{
	if (show == false)
		return true; 

	if (ImGui::Begin("Random Generator", &show, NULL))
	{	
		
		ImGui::TextWrapped("This engine is using simplified PCG library to generate random number creation. Select Help->Libraries to get extense information about it.");
		int rt = (int)random_type; 
		ImGui::Combo("Number Type", &rt, "Integer\0Float");
		random_type = (RandomNumberType)rt;

		if (random_type == RND_TYPE_INT)
		{
			ImGui::InputInt("Min", &min);
			ImGui::InputInt("Max", &max);		
		}
		else
		{
			ImGui::InputFloat("Min", &minf, 0.1f, 0.5f, 2);
			ImGui::InputFloat("Max", &maxf, 0.1f, 0.5f, 2);
		}

		ImGui::Text("Bounded Variable: XXX"); 

		if (show_warning)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Max & Min values should not match.");
			ImGui::SameLine(); 

			if (ImGui::Button("OK"))
				show_warning = false; 

			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Max should be > Min.");

			ImGui::End();
			return true;
		}

		if (ImGui::Button("Generate"))
		{
	
			if (random_type == RND_TYPE_INT)
			{
				if (max <= min)
					show_warning = true; 
				else
					generated_num = GetPCGRandomNumberInt(max, min);
			}
				
			else 
			{
				if (maxf <= minf)
					show_warning = true;
				else
					generated_num = GetPCGRandomNumberFloat(maxf, minf);
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
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Number: %.2f", generated_num);
		}

	}

	ImGui::End();

	return true;
}

#include "UI_ExplorerPanel.h"
#include "Application.h"

UI_ExplorerPanel::UI_ExplorerPanel()
{
	show_item_options = false; 
}


UI_ExplorerPanel::~UI_ExplorerPanel()
{
}

void UI_ExplorerPanel::DrawExplorerRecursive(std::string folder)
{
	string item_name; 

	if (App->file_system->IsFolder(folder.c_str()))
	{
		item_name = App->file_system->GetLastPathItem(folder.c_str(), true);

		ImGui::Image((ImTextureID)App->resources->material_importer->GetCheckerTexture()->GetTextureID(), ImVec2(15, 15)); ImGui::SameLine();

		if (ImGui::TreeNodeEx(item_name.c_str()))
		{
			//If it has childs we call them recursively
			vector<string> child_files = App->file_system->GetFilesInDirectory(folder.c_str());

			for (auto it = child_files.begin(); it != child_files.end(); it++)
			{
				if ((*it) != "." && (*it) != "..")
				{
					string next_file = folder + '\\' + (*it);
					DrawExplorerRecursive(next_file);
				}
					
			}				
			ImGui::TreePop();
		}
	}
	else
	{
		item_name = App->file_system->GetLastPathItem(folder.c_str(), true);

		ImGui::Image((ImTextureID)App->resources->material_importer->GetCheckerTexture()->GetTextureID(), ImVec2(15, 15)); ImGui::SameLine(); 

		ImGui::MenuItem(item_name.c_str());

		if (ImGui::IsItemClicked(1))
		{
			show_item_options = true;
			ImGui::OpenPopup("ExplorerOptions");
			item_selected = item_name; 
		}		
	}

	if (show_item_options && item_name == item_selected)
	{	
		if (ImGui::BeginPopup("ExplorerOptions"))
		{
			if (ImGui::MenuItem("Load Item"))
			{
			}

			if (ImGui::MenuItem("Delete Resource")) 
			{
			}

			if (ImGui::MenuItem("Open In Explorer"))
			{
			}
			
			ImGui::EndPopup();
		}
	}

	return; 
}

bool UI_ExplorerPanel::Start()
{
	show = true; 
	root_folder = App->file_system->GetAssetsPath(); 
	item_selected = ""; 

	return true;
}

bool UI_ExplorerPanel::Update()
{
	if (ImGui::Begin("Explorer", &show, NULL))
	{
		DrawExplorerRecursive(root_folder);

		ImGui::End();
	}
	return true;
}

bool UI_ExplorerPanel::CleanUp()
{
	return true;
}

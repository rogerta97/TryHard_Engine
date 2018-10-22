#include "UI_ExplorerPanel.h"
#include "Application.h"

UI_ExplorerPanel::UI_ExplorerPanel()
{
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
		item_name = App->file_system->GetLastPathItem(folder.c_str());
		ImGui::TreeNodeEx(item_name.c_str(), ImGuiTreeNodeFlags_Bullet);
	}

	return; 
}

bool UI_ExplorerPanel::Start()
{
	show = true; 
	root_folder = App->file_system->GetAssetsPath(); 

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

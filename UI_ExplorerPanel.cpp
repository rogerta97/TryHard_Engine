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

	//if (App->file_system->IsFolder(folder.c_str()))
	//{
	//	item_name = App->file_system->GetLastPathItem(folder.c_str(), false);
	//	string next_path = folder + item_name; 

	//	ImGui::TreeNodeEx(item_name.c_str());
	//	DrawExplorerRecursive(next_path);

	//	//If it has childs we call them recursively
	//}
	//else
	//{
	//	item_name = App->file_system->GetLastPathItem(folder.c_str(), false);
	//	ImGui::TreeNodeEx(item_name.c_str(), ImGuiTreeNodeFlags_Bullet);
	//}

	

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

#include "UI_HierarchyPanel.h"

#include "imgui_dock.h"
#include "Application.h"

UI_HierarchyPanel::UI_HierarchyPanel()
{
}


UI_HierarchyPanel::~UI_HierarchyPanel()
{
}

bool UI_HierarchyPanel::Start()
{
	show = true; 

	return true;
}

bool UI_HierarchyPanel::Update()
{
	if (ImGui::BeginDock("Hierarchy", &show, NULL))
	{
		static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
		static int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 2); // Increase spacing to differentiate leaves from expanded contents.

		int id = -1; 

		for (auto it = App->scene->scene_gameobjects.begin(); it != App->scene->scene_gameobjects.end(); it++)
		{
			if ((*it)->parent != nullptr)
				continue; 

			else
			{
				if ((*it)->HasChilds())
				{			
					(*it)->PrintHierarchyRecursive(selection_mask, node_clicked, id);													
				}
				else
				{
					if (ImGui::MenuItem((*it)->name.c_str()))
					{
						ImGui::TreePop();
					}
					
					id++; 
				}
			}

			if (node_clicked != -1)
			{
				selection_mask = (1 << node_clicked); 
			}
		}
	
		ImGui::PopStyleVar();

		

	//	static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
	//	int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
	//	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
	//	for (int i = 0; i < 6; i++)
	//	{
	//		// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
	//		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
	//		if (i < 3)
	//		{
	//			// Node
	//			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
	//			if (ImGui::IsItemClicked())
	//				node_clicked = i;
	//			if (node_open)
	//			{
	//				ImGui::Text("Blah blah\nBlah Blah");
	//				ImGui::TreePop();
	//			}
	//		}
	//		else
	//		{
	//			// Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
	//			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
	//			ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Leaf %d", i);
	//			if (ImGui::IsItemClicked())
	//				node_clicked = i;
	//		}
	//	}
	//	if (node_clicked != -1)
	//	{
	//		// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
	//		if (ImGui::GetIO().KeyCtrl)
	//			selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
	//		else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
	//			selection_mask = (1 << node_clicked);           // Click to single-select
	//	}
	//	ImGui::PopStyleVar();
	///*	if (align_label_with_current_x_position)
	//		ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());*/
	//}

	}

	ImGui::EndDock(); 

	return true;
}

bool UI_HierarchyPanel::CleanUp()
{
	return true;
}

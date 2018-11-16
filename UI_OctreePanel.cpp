#include "UI_OctreePanel.h"
#include "Application.h"
#include "Octree.h"

UI_OctreePanel::UI_OctreePanel()
{
	name = "Octree";
}


UI_OctreePanel::~UI_OctreePanel()
{
}

bool UI_OctreePanel::Start()
{
	show = true;
	octree = App->scene->octree; 
	size = 100.0f; 
	
	return true;
}

bool UI_OctreePanel::Update()
{
	if (ImGui::Begin("Octree"))
	{
		ImGui::TextWrapped("Octree is an space partitioning tool for optimizing frustum culling and intersection test. Creating an octree will take in account every static gameobject laying on the current scene"); 

		SEPARATE_WITH_SPACE

		ImGui::Checkbox("Adaptative", &octree->adaptative); ImGui::SameLine();
		ImGui::Checkbox("Draw Divisions", &octree->draw);

		if (octree->adaptative == false)
		{
			ImGui::InputFloat("Box Size", &size, 0.1f);

			if (size < 1)
				size = 1;
		}
	
		static int max_in_box = 1;
		ImGui::InputInt("Max in Box", &max_in_box);

		if (max_in_box < 1)
			max_in_box = 1; 

		ImGui::Spacing(); 
		if(octree)
			ImGui::Text("Objects: %d", octree->GetNumObjects()); 
		else
			ImGui::Text("Objects: 0");

		SEPARATE_WITH_SPACE

		if (octree->GetRoot() == nullptr)
		{
			if (ImGui::Button("Create"))
			{
				if (App->scene->static_gameobjects.empty())
				{
					CONSOLE_ERROR("Octree can not be created with any static GO in the scene.");
				}
				else
				{
					AABB octree_root;

					octree_root.minPoint = { -size, -size, -size };
					octree_root.maxPoint = { size, size, size };

					App->scene->octree->Create(octree_root, octree->adaptative, max_in_box);
					size = App->scene->octree->GetRoot()->box.Edge(0).Length() / 2;

				}


			}
		}
		else
		{
			if (ImGui::Button("Delete"))
			{
				octree->CleanUp();
			}
		}
	
		ImGui::End(); 

	}
	return true;
}

bool UI_OctreePanel::CleanUp()
{
	return true;
}

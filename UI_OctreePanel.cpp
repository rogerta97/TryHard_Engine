#include "UI_OctreePanel.h"
#include "Application.h"
#include "Octree.h"

UI_OctreePanel::UI_OctreePanel()
{
}


UI_OctreePanel::~UI_OctreePanel()
{
}

bool UI_OctreePanel::Start()
{
	show = true; 
	octree = App->scene->octree; 
	

	return true;
}

bool UI_OctreePanel::Update()
{
	if (ImGui::Begin("Octree"))
	{
		ImGui::TextWrapped("Octree is an space partitioning tool for optimizing frustum culling and intersection test. Creating an octree will take in account every static gameobject laying on the current scene"); 

		SEPARATE_WITH_SPACE

		static float size = 20.0f; 
		ImGui::InputFloat("Box Size", &size, 0.1f);

		ImGui::Checkbox("Adaptative", &octree->adaptative);

		ImGui::Spacing(); 
		ImGui::Text("Objects: %d", octree->GetNumObjects()); 

		SEPARATE_WITH_SPACE

		if (ImGui::Button("Create"))
		{
			AABB octree_root;
			octree_root.minPoint = { -size, -size, -size };
			octree_root.maxPoint = { size, size, size };

			App->scene->octree->Create(octree_root, octree->adaptative);
		} ImGui::SameLine(); 

		if (ImGui::Button("Delete"))
		{
			octree->CleanUp(); 
		}

		ImGui::End(); 

	}
	return true;
}

bool UI_OctreePanel::CleanUp()
{
	return true;
}

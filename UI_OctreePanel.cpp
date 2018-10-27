#include "UI_OctreePanel.h"
#include "Application.h"


UI_OctreePanel::UI_OctreePanel()
{
}


UI_OctreePanel::~UI_OctreePanel()
{
}

bool UI_OctreePanel::Start()
{
	show = true; 

	return true;
}

bool UI_OctreePanel::Update()
{

	if (ImGui::Begin("Octree"))
	{
		if (ImGui::Button("Create"))
		{
			AABB octree_root;
			octree_root.minPoint = { -10, -10, -10 };
			octree_root.maxPoint = { 10, 10, 10 };

			App->scene->octree->Create(octree_root);
		}

		ImGui::End(); 

	}
	return true;
}

bool UI_OctreePanel::CleanUp()
{
	return true;
}

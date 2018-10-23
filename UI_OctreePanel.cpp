#include "UI_OctreePanel.h"



UI_OctreePanel::UI_OctreePanel()
{
}


UI_OctreePanel::~UI_OctreePanel()
{
}

bool UI_OctreePanel::Start()
{
	show = false; 

	return true;
}

bool UI_OctreePanel::Update()
{

	if (ImGui::Begin("Octree"))
	{
		ImGui::Text("Octree"); 
		ImGui::End(); 
	}
	return true;
}

bool UI_OctreePanel::CleanUp()
{
	return true;
}

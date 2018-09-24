#include "ComponentMesh.h"

#include "imgui.h"
#include "OpenGL.h"


ComponentMesh::ComponentMesh()
{
	draw_mesh = true; 
	component_type = CMP_RENDERER; 
	render_settings_modified = false; 

	render_settings.cull_face = true; 
	render_settings.depth_test = true; 
	render_settings.color_material = true; 
	render_settings.wireframe = false; 
}


ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update()
{
	if (draw_mesh == false)
		return false;

	SetDrawSettings(); 
	mesh->DrawMesh();
	SetDefaultSettings(); 

	return true; 
}

void ComponentMesh::SetMesh(Mesh * new_mesh)
{
	mesh = new_mesh;
}

void ComponentMesh::SetDrawSettings()
{
	//Make needed render changes just in case is needed, if not the engine will render as default 

		if (render_settings.depth_test) 
			glEnable(GL_DEPTH_TEST);
		else  glDisable(GL_DEPTH_TEST);

		if (render_settings.cull_face) 
			glEnable(GL_CULL_FACE);
		else  glDisable(GL_CULL_FACE);

		if (render_settings.color_material) 
			glEnable(GL_COLOR_MATERIAL);
		else  glDisable(GL_COLOR_MATERIAL);

		if (render_settings.wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	
}

void ComponentMesh::SetDefaultSettings()
{	
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ComponentMesh::PrintRenderSettings()
{
	if (ImGui::TreeNode("Render Settings"))
	{
		//Draw info
		if (ImGui::Checkbox("Depth Test", &render_settings.depth_test))
			render_settings_modified = true; 
			
		//ImGui::SameLine();
		if (ImGui::Checkbox("Cull Face", &render_settings.cull_face))
			render_settings_modified = true;

		//ImGui::SameLine();
		if (ImGui::Checkbox("Wireframe", &render_settings.wireframe))
			render_settings_modified = true;

		//ImGui::SameLine();
		if (ImGui::Checkbox("Color Material", &render_settings.color_material))
			render_settings_modified = true;

		ImGui::TreePop();
	}

	
}

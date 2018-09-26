#include "ComponentMesh.h"
#include "Application.h"

#include "imgui.h"
#include "OpenGL.h"


ComponentMesh::ComponentMesh()
{
	draw_mesh = true; 
	component_type = CMP_RENDERER; 

	wireframe = false; 
}


ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update()
{
	SetDrawSettings();

	if (draw_mesh == false)
		return false;

	glColor3f(DEFAULT_GEOMETRY_COLOR);
	mesh->DrawMesh();

	//if the mesh is selected we draw it again in wireframe mode
	if (gameobject->selected)
	{
		glColor3f(DEFAULT_WIREFRAME_COLOR);
		wireframe = true;
		SetDrawSettings();
		mesh->DrawMesh();
		wireframe = false;
	}

	return true; 
}

void ComponentMesh::SetMesh(Mesh * new_mesh)
{
	mesh = new_mesh;
}

void ComponentMesh::SetDrawSettings()
{
	//Make needed render changes just in case is needed, if not the engine will render as default 

	if (wireframe)
	{
		glLineWidth(4.0f); 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1.0f);
	}
}

void ComponentMesh::SetDefaultSettings()
{	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ComponentMesh::PrintRenderSettings()
{
	ImGui::Checkbox("Wireframe", &wireframe);
}

#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "Application.h"

#include "imgui.h"
#include "OpenGL.h"


ComponentMesh::ComponentMesh()
{
	component_type = CMP_RENDERER; 

	material = nullptr;
	mesh = nullptr; 

	wireframe = false; 
	draw_mesh = true;
}


ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update()
{
	
	if (draw_mesh == false)
		return false;

	SetDrawSettings();
	DrawMesh();

	//if the mesh is selected we draw it again in wireframe mode
	if (gameobject->selected && wireframe == false)
	{
		wireframe = true;
		SetDrawSettings();
		DrawMesh();
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
		glLineWidth(3.0f);
		glColor3f(DEFAULT_WIREFRAME_COLOR);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	
	else
	{
		glLineWidth(2.0f);
		glColor3f(DEFAULT_GEOMETRY_COLOR);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	}
}

void ComponentMesh::DrawMesh()
{
	bool mat_active = false; 

	if (material != nullptr && wireframe)
		mat_active = true;

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);

	if (mat_active) 
	{
		material->GetDiffuseTexture()->Bind();
		glEnable(GL_TEXTURE_2D); 
	}
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (mat_active)
	{
		material->GetDiffuseTexture()->UnBind();
		glDisable(GL_TEXTURE_2D);
	}
	
	glDisableClientState(GL_VERTEX_ARRAY);
}

void ComponentMesh::SetDefaultSettings()
{	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ComponentMesh::PrintRenderSettings()
{
	ImGui::Checkbox("Wireframe", &wireframe);
}

void ComponentMesh::AssignMaterial(ComponentMaterial * new_mat)
{
	material = new_mat; 
}

ComponentMaterial * ComponentMesh::GetMaterial()
{
	return material;
}

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
	draw_normals = false;

	active = true;
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
	if (gameobject->selected && wireframe == false && App->renderer3D->render_settings.wireframe_selected == true)
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

void ComponentMesh::DrawNormals()
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);

	for (int i = 0; i < mesh->num_normals; i++)
	{
		LineSegment curr_line; 
		curr_line.a = mesh->vertices[i]; 
		curr_line.b = mesh->vertices[i] + (mesh->normal_cords[i]*0.5f);

		glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
		glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);	

		
	}

	glEnd();
	 
}

void ComponentMesh::DrawMesh()
{
	bool mat_active = false; 

	if (material != nullptr && wireframe == false && material->active)
		mat_active = true;

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	if (mat_active) 
	{
		if(material->GetDiffuseTexture() != nullptr)
			material->GetDiffuseTexture()->Bind();
		
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->uvs_id);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);	
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);

	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	if (draw_normals)
		DrawNormals(); 

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	if (mat_active)
	{
		material->GetDiffuseTexture()->UnBind();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);	

	if (draw_bounding_box)
		DrawBoundingBox(); 
}

void ComponentMesh::SetDefaultSettings()
{	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ComponentMesh::PrintRenderSettings()
{
	ImGui::Spacing(); 

	ImGui::Text("Vertices:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh->num_vertices);

	ImGui::Text("Index:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh->num_indices);

	ImGui::Text("TexCoords:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh->num_uvs*2);

	ImGui::Text("Normals: "); ImGui::SameLine(); ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%d", mesh->num_normals);

	ImGui::Spacing(); 
	ImGui::Separator(); 
	ImGui::Spacing();

	ImGui::Checkbox("Wireframe", &wireframe); ImGui::SameLine(); ImGui::Checkbox("Draw Normals", &draw_normals); 
}

void ComponentMesh::AssignMaterial(ComponentMaterial * new_mat)
{
	material = new_mat; 
}

Mesh * ComponentMesh::GetMesh() const
{
	return mesh;
}

bool ComponentMesh::CreateEnclosedMeshAABB()
{
	//Get Vertices of the mesh 
	ComponentMesh* mesh_cmp = (ComponentMesh*)gameobject->GetComponent(CMP_RENDERER);

	if (mesh_cmp != nullptr)
	{
		bounding_box.SetNegativeInfinity();
		bounding_box = bounding_box.MinimalEnclosingAABB(mesh_cmp->GetMesh()->vertices, mesh_cmp->GetMesh()->num_vertices);
		return true;
	}

	return false;
}

void ComponentMesh::DrawBoundingBox()
{
	//Draw The AABB
	if (gameobject->selected && draw_bounding_box)
	{
		LineSegment curr_line;

		glBegin(GL_LINES);
		glColor3f(0, 1, 0);

		for (int i = 0; i < 12; i++)
		{
			curr_line = bounding_box.Edge(i);

			glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
			glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);
		}

		glEnd();
	}
}

void ComponentMesh::SetBBColor(float r, float g, float b)
{
	glColor3f(r,g,b); 
}



ComponentMaterial * ComponentMesh::GetMaterial() const
{
	return material;
}

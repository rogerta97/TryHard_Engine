#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

#include "Application.h"
#include "GameObject.h"

#include "imgui.h"
#include "OpenGL.h"


ComponentMesh::ComponentMesh(GameObject* parent)
{
	SetGameObject(parent);
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
	if (App->camera->frustum_culling)
		frustum_col_type = App->camera->GetEditorCamera()->camera->IsAABBInside(bounding_box);
	else
		frustum_col_type = INSIDE_FRUSTUM;

	
	
	//
	if (draw_mesh == false || mesh == nullptr)
		return false;

	if (wireframe == false)
	{
		App->renderer3D->UseCurrentRenderSettings();
		DrawMesh();
	}
	
	//if the mesh is selected we draw it again in wireframe mode
	if (gameobject->selected && wireframe == false && App->renderer3D->render_settings.wireframe_selected == true)
	{
		wireframe = true;
		App->renderer3D->UseDebugRenderSettings();
		glLineWidth(3.0f); 
		DrawMesh();
		App->renderer3D->UseCurrentRenderSettings();
		wireframe = false;
	}

	if (draw_normals)
		DrawNormals();

	if (draw_bounding_box)
		DrawBoundingBox();

	return true;	
}

bool ComponentMesh::CleanUp()
{
	if (mesh != nullptr)
	{
		mesh->CleanMeshData();
		mesh = nullptr; 
	}

	if(material != nullptr)
		material->CleanUp();
		
	return false;
}

void ComponentMesh::SetMesh(Mesh * new_mesh)
{
	mesh = new_mesh;
}

void ComponentMesh::DrawNormals()
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);

	ComponentTransform* trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM);

	for (int i = 0; i < mesh->num_normals; i++)
	{
		LineSegment curr_line; 

		curr_line.a = mesh->vertices[i]; 
		curr_line.b = mesh->vertices[i] + (mesh->normal_cords[i]*0.5f);

		curr_line.Transform(trans->GetGlobalViewMatrix());

		glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
		glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);			
	}

	glEnd();
	 
}

void ComponentMesh::DrawMesh()
{
	if (frustum_col_type == OUTSIDE_FRUSTUM)
		return;
	ComponentMaterial* material = (ComponentMaterial*)gameobject->GetComponent(CMP_MATERIAL); 
	ComponentTransform* trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM);

	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	float4x4 view_mat = float4x4::identity;

	if (trans)
	{
		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		view_mat.Set((float*)matrix);

		glMatrixMode(GL_MODELVIEW); 
		glLoadMatrixf((GLfloat*)(trans->GetGlobalViewMatrix().Transposed() * view_mat).v);
	}

	bool valid_mat = false; 
	if (material)
	{
		if (material->GetMaterial()->GetDiffuseTexture() != nullptr)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->uvs_id);

			material->GetMaterial()->GetDiffuseTexture()->Bind();

			if (mesh->GetType() == MESH_FBX)
				glTexCoordPointer(3, GL_FLOAT, 0, NULL);

			else
				glTexCoordPointer(2, GL_FLOAT, 0, NULL);

			valid_mat = true; 

			if (mesh->num_normals != 0)
			{
				glEnableClientState(GL_NORMAL_ARRAY);

				glBindBuffer(GL_ARRAY_BUFFER, mesh->normals_id);
				glNormalPointer(GL_FLOAT, 0, NULL);
			}
		}		
	}
	
	if(!valid_mat)
	{
		glDisable(GL_TEXTURE_2D); 
		glColor3f(0.5f, 0.5f, 1.0f);
	}

	if(wireframe)
		glColor3f(DEFAULT_WIREFRAME_COLOR);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	if (material)
		material->GetMaterial()->GetDiffuseTexture()->UnBind();
	
	if (trans)
	{
		trans->DrawAxis(); 
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)view_mat.v);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
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
	bounding_box.SetNegativeInfinity();
	bounding_box = bounding_box.MinimalEnclosingAABB(GetMesh()->vertices, GetMesh()->num_vertices);
	return true;	
}

void ComponentMesh::DrawBoundingBox()
{
	//Draw The AABB
	if (gameobject->selected && draw_bounding_box)
	{
		LineSegment curr_line;

		glBegin(GL_LINES);		
		App->renderer3D->UseDebugRenderSettings(); 
		glColor3f(1.0f, 0.0f, 0.0f);

		UpdateBoundingBox(); 

		for (int i = 0; i < 12; i++)
		{
			curr_line = bounding_box.Edge(i);

			glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
			glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);
		}

		glEnd();
	}
}

void ComponentMesh::UpdateBoundingBox()
{
	ComponentTransform* trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM);

	if (trans)
	{
		bounding_box.SetNegativeInfinity();
		bounding_box = bounding_box.MinimalEnclosingAABB(mesh->vertices, mesh->num_vertices);
		bounding_box.TransformAsAABB(trans->GetGlobalViewMatrix());
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

void ComponentMesh::SetMaterial(ComponentMaterial * mat)
{
	material = mat; 
}

void ComponentMesh::DeleteMaterial()
{
	if(material != nullptr)
		material->CleanUp(); 

	material = nullptr; 
}

void ComponentMesh::CheckAABBPoints(float3 & min_point, float3 & max_point)
{
	if (bounding_box.minPoint.Distance({0,0,0}) >= min_point.Distance({ 0,0,0 }))
		min_point = bounding_box.minPoint;

	if (bounding_box.maxPoint.Distance({ 0,0,0 }) >= max_point.Distance({ 0,0,0 }))
		max_point = bounding_box.maxPoint;	
}

#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentRectTransform.h"
#include "ComponentCanvas.h"
#include "ComponentTransform.h"

#include "Application.h"
#include "GameObject.h"

#include "MeshImporter.h"

#include "imgui.h"
#include "OpenGL.h"


#include "mmgr\mmgr.h"


ComponentMesh::ComponentMesh(GameObject* parent)
{
	SetGameObject(parent);
	component_type = CMP_MESH; 

	material = nullptr;
	mesh = nullptr; 

	wireframe = false; 
	draw_mesh = true;
	draw_normals = false;

	container_fbx = ""; 

	active = true;
}


ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update()
{
	if (draw_mesh == false || mesh == nullptr)
		return false;

	ComponentTransform* trans = nullptr; 

	if (gameobject->GetIsUI())
	{
		ComponentRectTransform* rect_trans = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
		trans = rect_trans->GetTransform();		
	}		
	else
		trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM);

	if (trans->HasTransformed()) {
		UpdateBoundingBox();
		trans->SetHasTransformed(false);
	}

	return true;	
}

bool ComponentMesh::CleanUp()
{
	if (mesh != nullptr)
	{
		mesh->reference_counting--; 
		mesh->CleanMeshData();
		mesh = nullptr; 
	}
		
	return false;
}



void ComponentMesh::SetMesh(Mesh * new_mesh)
{
	mesh = new_mesh;
}

void ComponentMesh::DrawNormals()
{
	if (mesh->num_normals == 0)
		return; 

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
	if (frustum_col_type == OUTSIDE_FRUSTUM || mesh == nullptr)
		return;

	ComponentMaterial* material = (ComponentMaterial*)gameobject->GetComponent(CMP_MATERIAL); 
	ComponentTransform* trans = nullptr;

	if (gameobject->GetIsUI() == false)
		trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM);
	else
	{
		ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
		trans = rtransform->GetTransform();		
	}
		
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
		glLoadMatrixf((GLfloat*)((trans->GetGlobalViewMatrix()).Transposed() * view_mat).v);
	}

	bool valid_mat = false; 
	if (material && material->GetMaterial())
	{
		valid_mat = true;

		if (material->GetMaterial()->GetDiffuseTexture() != nullptr)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->uvs_id);

			material->GetMaterial()->GetDiffuseTexture()->Bind();
	
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		
			if (mesh->num_normals != 0)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->normals_id);
				glNormalPointer(GL_FLOAT, 0, NULL);
			}
		}
		else 
		{
			glDisable(GL_TEXTURE_2D);
			Color color = material->GetMaterial()->color;
			glColor3f(color.r, color.g, color.b);
		}
	}
	else if(!valid_mat)
	{
		glDisable(GL_TEXTURE_2D); 
		glColor3f(0.5f, 0.5f, 1.0f);
	}

	if (wireframe)
	{
		App->renderer3D->UseDebugRenderSettings();
	}
		

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	if (valid_mat)
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

void ComponentMesh::Draw(bool is_editor)
{
	if (wireframe == false)
	{
		App->renderer3D->UseCurrentRenderSettings();
		DrawMesh();
	}

	//if the mesh is selected we draw it again in wireframe mode
	if (gameobject->selected && App->renderer3D->render_settings.wireframe_selected == true && is_editor == true)
	{	
		App->renderer3D->UseDebugRenderSettings();
		glLineWidth(3.0f);
		DrawMesh();
		App->renderer3D->UseCurrentRenderSettings();	
	}

	if (draw_normals)
		DrawNormals();

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
	bounding_box.SetNegativeInfinity();
	bounding_box = bounding_box.MinimalEnclosingAABB(GetMesh()->vertices, GetMesh()->num_vertices);

	if(gameobject)
		gameobject->bounding_box = &bounding_box; 

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

		for (int i = 0; i < 12; i++)
		{
			curr_line = bounding_box.Edge(i);

			glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
			glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);
		}

		glEnd();
	}
}

void ComponentMesh::UpdateBoundingBox(ComponentTransform* force_trans)
{
	ComponentTransform* trans = nullptr;
	
	if (force_trans)		
		trans = force_trans;		
	else
		trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM);
	

	if (trans && mesh)
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

void ComponentMesh::SetWireframe(bool newValue)
{
	wireframe = newValue; 
}

bool ComponentMesh::GetClosestIntersectionPoint(LineSegment line, float3 &closest_point, float & distance)
{
	ComponentTransform* trans = (ComponentTransform*)gameobject->GetComponent(CMP_TRANSFORM);
	Triangle tri;
	float current_distance;
	float closest_distance = 10000; //Hmm maybe there is a better way to do this
	float3 hit_point;
	bool ret = false;

	if (!mesh)
		return false;

	if (!mesh->vertices)
		return false;

	int num_tris = mesh->num_indices / 3;

	float4x4 gm = trans->GetGlobalViewMatrix();


	line.Transform(gm.Inverted());

	for (int i = 0; i < mesh->num_indices; i+=3)
	{
		float3 vertex_a = mesh->vertices[mesh->indices[i]];
		float3 vertex_b = mesh->vertices[mesh->indices[i + 1]];
		float3 vertex_c = mesh->vertices[mesh->indices[i + 2]];

		tri.a = vertex_a;
		tri.b = vertex_b;
		tri.c = vertex_c;

		bool hit = line.Intersects(tri, &current_distance, &hit_point);

		if (hit) 
		{
			//CONSOLE_LOG("hitpoint:  x:%f, y:%f, z:%f", hit_point.x, hit_point.y, hit_point.z);
			if (current_distance < closest_distance)
			{
				closest_point = hit_point;
				closest_distance = current_distance;
				ret = true;
			}
		}
	}

	closest_point += gm.TranslatePart();
	distance = closest_distance;

	//if (ret)
	//	CONSOLE_LOG("x:%f, y:%f, z:%f distance:%f", closest_point.x, closest_point.y, closest_point.z, closest_distance);
	return ret;
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

void ComponentMesh::Load(JSON_Object * root_obj, UID prefab_uid)
{
	string container_fbx = json_object_dotget_string(root_obj, "FBXName");
	string meta_path = json_object_dotget_string(root_obj, "MetaName");
	string mesh_name = App->file_system->GetLastPathItem(meta_path, true);

	///Save somehow from what FBX the mesh is comming, if the Mesh don't exist in the library, 
	///load the FBX (not adding it to the scene) and unload it in order to create the library mesh. 
	///Then you can go on
	
	//Check if it's in library
	if (!App->file_system->IsFileInDirectory(App->file_system->GetModelsPath() + "\\MetaMeshes", mesh_name.c_str()))
	{
		//Load the containing FBX && Unload It
		GameObject* new_go = App->resources->mesh_importer->CreateFBXMesh(string(App->file_system->GetModelsPath() + string("\\") + container_fbx + ".fbx").c_str(), prefab_uid, true);

		if (new_go)
		{
			new_go->DeleteRecursive();
			App->scene->DeleteGameObjectsNow();
		}			
		else
		{
			CONSOLE_ERROR("Object '%s' could not be found in Assets folder. The '%s'.fbx resource has been deleted or renamed", gameobject->name.c_str(), container_fbx.c_str());
			return; 
		}
	}
		
	//Load Library Resource
	SetMesh(App->resources->mesh_importer->LoadFromBinary(meta_path.c_str()));
	mesh->type = MESH_FBX;

	if(mesh->reference_counting == 0)
		mesh->LoadToMemory();

	draw_bounding_box = false; 

	mesh->reference_counting++;
}

void ComponentMesh::Save(JSON_Object * root_obj, const char* root)
{
	std::string node_name = root;
	std::string item_name = "";

	item_name = node_name + ".ComponentMesh.MeshName";
	json_object_dotset_string(root_obj, item_name.c_str(), gameobject->name.c_str());

	item_name = node_name + ".ComponentMesh.MetaName";
	json_object_dotset_string(root_obj, item_name.c_str(), mesh->meta_path.c_str());

	item_name = node_name + ".ComponentMesh.FBXName";
	json_object_dotset_string(root_obj, item_name.c_str(), container_fbx.c_str());

	item_name = node_name + ".ComponentMesh.Type";
	json_object_dotset_string(root_obj, item_name.c_str(), ".fbx");
}

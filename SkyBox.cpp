#include "SkyBox.h"
#include "Application.h"
#include "Mesh.h"

#include "Material.h"

#include "MaterialImporter.h"

#include "ComponentCamera.h"
#include "ComponentTransform.h"


#include "mmgr\mmgr.h"


SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
}

void SkyBox::InitSkyBox(const char * folder_name, float size)
{

	string curr_path;

	this->size = size;

	curr_path = App->file_system->GetSkyBoxPath() + string("\\") + "miramar_front.png";
	CreateFrontPlane(curr_path.c_str());

	curr_path = App->file_system->GetSkyBoxPath() + string("\\") + "miramar_right.png";
	CreateRightPlane(curr_path.c_str());

	curr_path = App->file_system->GetSkyBoxPath() + string("\\") + "miramar_left.png";
	CreateLeftPlane(curr_path.c_str());

	curr_path = App->file_system->GetSkyBoxPath() + string("\\") + "miramar_back.png";
	CreateBackPlane(curr_path.c_str());

	curr_path = App->file_system->GetSkyBoxPath() + string("\\") + "miramar_up.png";
	CreateTopPlane(curr_path.c_str());

	curr_path = App->file_system->GetSkyBoxPath() + string("\\") + "miramar_down.png";
	CreateDownPlane(curr_path.c_str());

	camera_attached = nullptr;
}

void SkyBox::AttachTo(GameObject * go)
{
	if (go != nullptr)
		camera_attached = go;
}

void SkyBox::Draw()
{
	glColor3f(1.0f, 1.0f, 1.0f); 

	glDisable(GL_LIGHTING); 
	glDisable(GL_DEPTH_TEST); 
	glEnable(GL_TEXTURE_2D); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float4x4 view_mat = float4x4::identity;

	if (camera_attached != nullptr)
	{
		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		view_mat.Set((float*)matrix);

		ComponentCamera* cam = (ComponentCamera*)camera_attached->GetComponent(CMP_CAMERA);
		
		float4x4 new_mat = float4x4::identity;
		new_mat = new_mat.Translate(cam->GetFrustum()->pos);
		new_mat.Transpose();

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)(new_mat * view_mat).v);
	}

	for (int i = 0; i < 6; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, sky_cube[i]->vertices_id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (sky_textures[i] != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, sky_textures[i]->GetTextureID());
			glBindBuffer(GL_ARRAY_BUFFER, sky_cube[i]->uvs_id);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}
	
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[i]->indices_id);
		glDrawElements(GL_TRIANGLES, sky_cube[i]->num_indices, GL_UNSIGNED_INT, NULL);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	if (camera_attached != nullptr)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)view_mat.v);
	}

	App->renderer3D->UseCurrentRenderSettings(); 
}

void SkyBox::CleanUpSkyBox()
{
	for (int i = 0; i < 6; i++)
		sky_cube[i]->CleanMeshData();

}

void SkyBox::CreateFrontPlane(const char* front_image_path)
{
	//Create Front Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_FRONT] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	string name_from_path = App->file_system->GetLastPathItem(front_image_path); 

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_FRONT]->num_vertices = 4;
	sky_cube[SKYBOX_FRONT]->vertices = new float3[4];
						  
	sky_cube[SKYBOX_FRONT]->vertices[0].x = size + 0.01f;
	sky_cube[SKYBOX_FRONT]->vertices[0].y = size + 0.01f;
	sky_cube[SKYBOX_FRONT]->vertices[0].z = size;
						  
	sky_cube[SKYBOX_FRONT]->vertices[1].x = -(size + 0.01f);
	sky_cube[SKYBOX_FRONT]->vertices[1].y = (size + 0.01f);
	sky_cube[SKYBOX_FRONT]->vertices[1].z = size;
						  
	sky_cube[SKYBOX_FRONT]->vertices[2].x = (size + 0.01f);
	sky_cube[SKYBOX_FRONT]->vertices[2].y = -(size + 0.01f);
	sky_cube[SKYBOX_FRONT]->vertices[2].z = size;
						  
	sky_cube[SKYBOX_FRONT]->vertices[3].x = -(size + 0.01f);
	sky_cube[SKYBOX_FRONT]->vertices[3].y = -(size + 0.01f);
	sky_cube[SKYBOX_FRONT]->vertices[3].z = size;

	// ---------------------

	sky_cube[SKYBOX_FRONT]->vertices_id = sky_cube[SKYBOX_FRONT]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_FRONT]->vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_FRONT]->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_FRONT]->num_indices = 6;
	sky_cube[SKYBOX_FRONT]->indices = new int[6];
						  
	sky_cube[SKYBOX_FRONT]->indices[0] = 2;
	sky_cube[SKYBOX_FRONT]->indices[1] = 1;
	sky_cube[SKYBOX_FRONT]->indices[2] = 0;
	sky_cube[SKYBOX_FRONT]->indices[3] = 2;
	sky_cube[SKYBOX_FRONT]->indices[4] = 3;
	sky_cube[SKYBOX_FRONT]->indices[5] = 1;

	// ---------------------

	sky_cube[SKYBOX_FRONT]->indices_id = sky_cube[SKYBOX_FRONT]->CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_FRONT]->indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_FRONT]->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Load The texture if it's valid 
	sky_cube[SKYBOX_FRONT]->num_uvs = sky_cube[SKYBOX_FRONT]->num_vertices;
	sky_cube[SKYBOX_FRONT]->uvs_cords = new float[sky_cube[SKYBOX_FRONT]->num_uvs * 2];
						  
	sky_cube[SKYBOX_FRONT]->uvs_cords[0] = 0.0f;
	sky_cube[SKYBOX_FRONT]->uvs_cords[1] = 1.0f;
						  
	sky_cube[SKYBOX_FRONT]->uvs_cords[2] = 1.0f;
	sky_cube[SKYBOX_FRONT]->uvs_cords[3] = 1.0f;
						  
	sky_cube[SKYBOX_FRONT]->uvs_cords[4] = 0.0f;
	sky_cube[SKYBOX_FRONT]->uvs_cords[5] = 0.0f;
						  
	sky_cube[SKYBOX_FRONT]->uvs_cords[6] = 1.0f;
	sky_cube[SKYBOX_FRONT]->uvs_cords[7] = 0.0f;

	Material* curr_mat = (Material*)App->resources->Get(RES_MATERIAL, name_from_path.c_str());

	if(curr_mat)
	{
		curr_mat->LoadToMemory();
		sky_textures[SKYBOX_FRONT] = curr_mat->GetDiffuseTexture();
		curr_mat->reference_counting++;
	}


	// ---------------------

	sky_cube[SKYBOX_FRONT]->uvs_id = sky_cube[SKYBOX_FRONT]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_FRONT]->uvs_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_FRONT]->uvs_cords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------
}

void SkyBox::CreateRightPlane(const char* plane_tex_path)
{
	//Create Right Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_RIGHT] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	string name_from_path = App->file_system->GetLastPathItem(plane_tex_path);

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_RIGHT]->num_vertices = 4;
	sky_cube[SKYBOX_RIGHT]->vertices = new float3[4];
						  
	sky_cube[SKYBOX_RIGHT]->vertices[0].x = -size;
	sky_cube[SKYBOX_RIGHT]->vertices[0].y = (size + 0.01f);
	sky_cube[SKYBOX_RIGHT]->vertices[0].z = (size + 0.01f);
						  
	sky_cube[SKYBOX_RIGHT]->vertices[1].x = -size;
	sky_cube[SKYBOX_RIGHT]->vertices[1].y = (size + 0.01f);
	sky_cube[SKYBOX_RIGHT]->vertices[1].z = -(size + 0.01f);
						  
	sky_cube[SKYBOX_RIGHT]->vertices[2].x = -size;
	sky_cube[SKYBOX_RIGHT]->vertices[2].y = -(size + 0.01f);
	sky_cube[SKYBOX_RIGHT]->vertices[2].z = (size + 0.01f);
						  
	sky_cube[SKYBOX_RIGHT]->vertices[3].x = -size;
	sky_cube[SKYBOX_RIGHT]->vertices[3].y = -(size + 0.01f);
	sky_cube[SKYBOX_RIGHT]->vertices[3].z = -(size + 0.01f);

	// ---------------------

	sky_cube[SKYBOX_RIGHT]->vertices_id = sky_cube[SKYBOX_RIGHT]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_RIGHT]->vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_RIGHT]->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_RIGHT]->num_indices = 6;
	sky_cube[SKYBOX_RIGHT]->indices = new int[6];
						  
	sky_cube[SKYBOX_RIGHT]->indices[0] = 1;
	sky_cube[SKYBOX_RIGHT]->indices[1] = 0;
	sky_cube[SKYBOX_RIGHT]->indices[2] = 2;
	sky_cube[SKYBOX_RIGHT]->indices[3] = 1;
	sky_cube[SKYBOX_RIGHT]->indices[4] = 2;
	sky_cube[SKYBOX_RIGHT]->indices[5] = 3;

	// ---------------------

	sky_cube[SKYBOX_RIGHT]->indices_id = sky_cube[SKYBOX_RIGHT]->CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_RIGHT]->indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_RIGHT]->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------

	//Load The texture if it's valid 
	sky_cube[SKYBOX_RIGHT]->num_uvs = sky_cube[SKYBOX_RIGHT]->num_vertices;
	sky_cube[SKYBOX_RIGHT]->uvs_cords = new float[sky_cube[SKYBOX_RIGHT]->num_uvs * 2];
			 			  
	sky_cube[SKYBOX_RIGHT]->uvs_cords[0] = 0.0f;
	sky_cube[SKYBOX_RIGHT]->uvs_cords[1] = 1.0f;
			 			  
	sky_cube[SKYBOX_RIGHT]->uvs_cords[2] = 1.0f;
	sky_cube[SKYBOX_RIGHT]->uvs_cords[3] = 1.0f;
			 			
	sky_cube[SKYBOX_RIGHT]->uvs_cords[4] = 0.0f;
	sky_cube[SKYBOX_RIGHT]->uvs_cords[5] = 0.0f;
			 			
	sky_cube[SKYBOX_RIGHT]->uvs_cords[6] = 1.0f;
	sky_cube[SKYBOX_RIGHT]->uvs_cords[7] = 0.0f;

	Material* curr_mat = (Material*)App->resources->Get(RES_MATERIAL, name_from_path.c_str());

	if(curr_mat != nullptr)
	{
		curr_mat->LoadToMemory();
		sky_textures[SKYBOX_RIGHT] = curr_mat->GetDiffuseTexture();
		curr_mat->reference_counting++;
	}

	// ---------------------

	sky_cube[SKYBOX_RIGHT]->uvs_id = sky_cube[SKYBOX_RIGHT]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_RIGHT]->uvs_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_RIGHT]->uvs_cords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkyBox::CreateLeftPlane(const char* plane_tex_path)
{
	//Create Left Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_LEFT] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	string name_from_path = App->file_system->GetLastPathItem(plane_tex_path);

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_LEFT]->num_vertices = 4;
	sky_cube[SKYBOX_LEFT]->vertices = new float3[4];
						 
	sky_cube[SKYBOX_LEFT]->vertices[0].x = size;
	sky_cube[SKYBOX_LEFT]->vertices[0].y = (size + 0.01f);
	sky_cube[SKYBOX_LEFT]->vertices[0].z = (size + 0.01f);
						 
	sky_cube[SKYBOX_LEFT]->vertices[1].x = size;
	sky_cube[SKYBOX_LEFT]->vertices[1].y = (size + 0.01f);
	sky_cube[SKYBOX_LEFT]->vertices[1].z = -(size + 0.01f);
						
	sky_cube[SKYBOX_LEFT]->vertices[2].x = size;
	sky_cube[SKYBOX_LEFT]->vertices[2].y = -(size + 0.01f);
	sky_cube[SKYBOX_LEFT]->vertices[2].z = (size + 0.01f);
						
	sky_cube[SKYBOX_LEFT]->vertices[3].x = size;
	sky_cube[SKYBOX_LEFT]->vertices[3].y = -(size + 0.01f);
	sky_cube[SKYBOX_LEFT]->vertices[3].z = -(size + 0.01f);

	// ---------------------

	sky_cube[SKYBOX_LEFT]->vertices_id = sky_cube[SKYBOX_LEFT]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_LEFT]->vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_LEFT]->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_LEFT]->num_indices = 6;
	sky_cube[SKYBOX_LEFT]->indices = new int[6];
						 
	sky_cube[SKYBOX_LEFT]->indices[0] = 0;
	sky_cube[SKYBOX_LEFT]->indices[1] = 1;
	sky_cube[SKYBOX_LEFT]->indices[2] = 2;
	sky_cube[SKYBOX_LEFT]->indices[3] = 2;
	sky_cube[SKYBOX_LEFT]->indices[4] = 1;
	sky_cube[SKYBOX_LEFT]->indices[5] = 3;

	// ---------------------

	sky_cube[SKYBOX_LEFT]->indices_id = sky_cube[SKYBOX_LEFT]->CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_LEFT]->indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_LEFT]->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------

	//Load The texture if it's valid 
	sky_cube[SKYBOX_LEFT]->num_uvs = sky_cube[SKYBOX_LEFT]->num_vertices;
	sky_cube[SKYBOX_LEFT]->uvs_cords = new float[sky_cube[SKYBOX_LEFT]->num_uvs * 2];
			 			 
	sky_cube[SKYBOX_LEFT]->uvs_cords[0] = 1.0f;
	sky_cube[SKYBOX_LEFT]->uvs_cords[1] = 1.0f;
			 			 
	sky_cube[SKYBOX_LEFT]->uvs_cords[2] = 0.0f;
	sky_cube[SKYBOX_LEFT]->uvs_cords[3] = 1.0f;
			 			 
	sky_cube[SKYBOX_LEFT]->uvs_cords[4] = 1.0f;
	sky_cube[SKYBOX_LEFT]->uvs_cords[5] = 0.0f;
			 			 
	sky_cube[SKYBOX_LEFT]->uvs_cords[6] = 0.0f;
	sky_cube[SKYBOX_LEFT]->uvs_cords[7] = 0.0f;

	Material* curr_mat = (Material*)App->resources->Get(RES_MATERIAL, name_from_path.c_str());

	if(curr_mat != nullptr)
	{
		curr_mat->LoadToMemory();
		sky_textures[SKYBOX_LEFT] = curr_mat->GetDiffuseTexture();
		curr_mat->reference_counting++;
	}

	// ---------------------

	sky_cube[SKYBOX_LEFT]->uvs_id = sky_cube[SKYBOX_LEFT]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_LEFT]->uvs_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_LEFT]->uvs_cords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkyBox::CreateBackPlane(const char* plane_tex_path)
{
	//Create Right Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_BACK] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	string name_from_path = App->file_system->GetLastPathItem(plane_tex_path);

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_BACK]->num_vertices = 4;
	sky_cube[SKYBOX_BACK]->vertices = new float3[4];
						 
	sky_cube[SKYBOX_BACK]->vertices[0].x = (size + 0.01f);
	sky_cube[SKYBOX_BACK]->vertices[0].y = (size + 0.01f);
	sky_cube[SKYBOX_BACK]->vertices[0].z = -size;
						 
	sky_cube[SKYBOX_BACK]->vertices[1].x = -(size + 0.01f);
	sky_cube[SKYBOX_BACK]->vertices[1].y = size;
	sky_cube[SKYBOX_BACK]->vertices[1].z = -(size + 0.01f);
						 
	sky_cube[SKYBOX_BACK]->vertices[2].x = (size + 0.01f);
	sky_cube[SKYBOX_BACK]->vertices[2].y = -(size + 0.01f);
	sky_cube[SKYBOX_BACK]->vertices[2].z = -size;
						 
	sky_cube[SKYBOX_BACK]->vertices[3].x = -(size + 0.01f);
	sky_cube[SKYBOX_BACK]->vertices[3].y = -(size + 0.01f);
	sky_cube[SKYBOX_BACK]->vertices[3].z = -size;

	// ---------------------

	sky_cube[SKYBOX_BACK]->vertices_id = sky_cube[SKYBOX_BACK]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_BACK]->vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_BACK]->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_BACK]->num_indices = 6;
	sky_cube[SKYBOX_BACK]->indices = new int[6];
						 
	sky_cube[SKYBOX_BACK]->indices[0] = 0;
	sky_cube[SKYBOX_BACK]->indices[1] = 1;
	sky_cube[SKYBOX_BACK]->indices[2] = 2;
	sky_cube[SKYBOX_BACK]->indices[3] = 2;
	sky_cube[SKYBOX_BACK]->indices[4] = 1;
	sky_cube[SKYBOX_BACK]->indices[5] = 3;

	// ---------------------

	sky_cube[SKYBOX_BACK]->indices_id = sky_cube[SKYBOX_BACK]->CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_BACK]->indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_BACK]->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------

	//Load The texture if it's valid 
	sky_cube[SKYBOX_BACK]->num_uvs = sky_cube[SKYBOX_BACK]->num_vertices;
	sky_cube[SKYBOX_BACK]->uvs_cords = new float[sky_cube[SKYBOX_BACK]->num_uvs * 2];
			 			 
	sky_cube[SKYBOX_BACK]->uvs_cords[0] = 1.0f;
	sky_cube[SKYBOX_BACK]->uvs_cords[1] = 1.0f;
			 			 
	sky_cube[SKYBOX_BACK]->uvs_cords[2] = 0.0f;
	sky_cube[SKYBOX_BACK]->uvs_cords[3] = 1.0f;
			 			 
	sky_cube[SKYBOX_BACK]->uvs_cords[4] = 1.0f;
	sky_cube[SKYBOX_BACK]->uvs_cords[5] = 0.0f;
			 			 
	sky_cube[SKYBOX_BACK]->uvs_cords[6] = 0.0f;
	sky_cube[SKYBOX_BACK]->uvs_cords[7] = 0.0f;

	Material* curr_mat = (Material*)App->resources->Get(RES_MATERIAL, name_from_path.c_str());

	if(curr_mat != nullptr)
	{
		curr_mat->LoadToMemory();
		sky_textures[SKYBOX_BACK] = curr_mat->GetDiffuseTexture();
		curr_mat->reference_counting++;
	}

	// ---------------------

	sky_cube[SKYBOX_BACK]->uvs_id = sky_cube[SKYBOX_BACK]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_BACK]->uvs_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_BACK]->uvs_cords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkyBox::CreateTopPlane(const char* plane_tex_path)
{
	//Create Right Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_TOP] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	string name_from_path = App->file_system->GetLastPathItem(plane_tex_path);

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_TOP]->num_vertices = 4;
	sky_cube[SKYBOX_TOP]->vertices = new float3[4];
			 			
	sky_cube[SKYBOX_TOP]->vertices[0].x = (size + 0.01f);
	sky_cube[SKYBOX_TOP]->vertices[0].y = size;
	sky_cube[SKYBOX_TOP]->vertices[0].z = (size + 0.01f);
			 			
	sky_cube[SKYBOX_TOP]->vertices[1].x = -(size + 0.01f);
	sky_cube[SKYBOX_TOP]->vertices[1].y = size;
	sky_cube[SKYBOX_TOP]->vertices[1].z = (size + 0.01f);
			 			
	sky_cube[SKYBOX_TOP]->vertices[2].x = (size + 0.01f);
	sky_cube[SKYBOX_TOP]->vertices[2].y = size;
	sky_cube[SKYBOX_TOP]->vertices[2].z = -(size + 0.01f);
			 			
	sky_cube[SKYBOX_TOP]->vertices[3].x = -(size + 0.01f);
	sky_cube[SKYBOX_TOP]->vertices[3].y = size;
	sky_cube[SKYBOX_TOP]->vertices[3].z = -(size + 0.01f);

	// ---------------------

	sky_cube[SKYBOX_TOP]->vertices_id = sky_cube[SKYBOX_TOP]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_TOP]->vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_TOP]->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------
						
	sky_cube[SKYBOX_TOP]->num_indices = 6;
	sky_cube[SKYBOX_TOP]->indices = new int[6];
			 			
	sky_cube[SKYBOX_TOP]->indices[0] = 2;
	sky_cube[SKYBOX_TOP]->indices[1] = 0;
	sky_cube[SKYBOX_TOP]->indices[2] = 1;
	sky_cube[SKYBOX_TOP]->indices[3] = 3;
	sky_cube[SKYBOX_TOP]->indices[4] = 2;
	sky_cube[SKYBOX_TOP]->indices[5] = 1;

	// ---------------------

	sky_cube[SKYBOX_TOP]->indices_id = sky_cube[SKYBOX_TOP]->CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_TOP]->indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_TOP]->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------

	//Load The texture if it's valid 
	sky_cube[SKYBOX_TOP]->num_uvs = sky_cube[SKYBOX_TOP]->num_vertices;
	sky_cube[SKYBOX_TOP]->uvs_cords = new float[sky_cube[SKYBOX_TOP]->num_uvs * 2];
						
	sky_cube[SKYBOX_TOP]->uvs_cords[0] = 1.0f;
	sky_cube[SKYBOX_TOP]->uvs_cords[1] = 0.0f;
						
	sky_cube[SKYBOX_TOP]->uvs_cords[2] = 1.0f;
	sky_cube[SKYBOX_TOP]->uvs_cords[3] = 1.0f;
						
	sky_cube[SKYBOX_TOP]->uvs_cords[4] = 0.0f;
	sky_cube[SKYBOX_TOP]->uvs_cords[5] = 0.0f;
						
	sky_cube[SKYBOX_TOP]->uvs_cords[6] = 0.0f;
	sky_cube[SKYBOX_TOP]->uvs_cords[7] = 1.0f;

	Material* curr_mat = (Material*)App->resources->Get(RES_MATERIAL, name_from_path.c_str());

	if(curr_mat != nullptr)
	{
		curr_mat->LoadToMemory();
		sky_textures[SKYBOX_TOP] = curr_mat->GetDiffuseTexture();
		curr_mat->reference_counting++;
	}

	// ---------------------

	sky_cube[SKYBOX_TOP]->uvs_id = sky_cube[SKYBOX_TOP]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_TOP]->uvs_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_TOP]->uvs_cords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SkyBox::CreateDownPlane(const char* plane_tex_path)
{
	//Create Right Plane  ------------------------------------------------------------

	sky_cube[SKYBOX_DOWN] = new Mesh();
	int type = BasicMeshType::MESH_PLANE;

	string name_from_path = App->file_system->GetLastPathItem(plane_tex_path);

	// Create vertices ------------------------------------

	sky_cube[SKYBOX_DOWN]->num_vertices = 4;
	sky_cube[SKYBOX_DOWN]->vertices = new float3[4];
						
	sky_cube[SKYBOX_DOWN]->vertices[0].x = (size + 0.01f);
	sky_cube[SKYBOX_DOWN]->vertices[0].y = -size;
	sky_cube[SKYBOX_DOWN]->vertices[0].z = (size + 0.01f);
			 			 
	sky_cube[SKYBOX_DOWN]->vertices[1].x = -(size + 0.01f);
	sky_cube[SKYBOX_DOWN]->vertices[1].y = -size;
	sky_cube[SKYBOX_DOWN]->vertices[1].z = (size + 0.01f);
			 			 
	sky_cube[SKYBOX_DOWN]->vertices[2].x = (size + 0.01f);
	sky_cube[SKYBOX_DOWN]->vertices[2].y = -size;
	sky_cube[SKYBOX_DOWN]->vertices[2].z = -(size + 0.01f);
			 			
	sky_cube[SKYBOX_DOWN]->vertices[3].x = -(size + 0.01f);
	sky_cube[SKYBOX_DOWN]->vertices[3].y = -size;
	sky_cube[SKYBOX_DOWN]->vertices[3].z = -(size + 0.01f);

	// ---------------------

	sky_cube[SKYBOX_DOWN]->vertices_id = sky_cube[SKYBOX_DOWN]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_DOWN]->vertices_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_DOWN]->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Create Indices --------------------------------------

	sky_cube[SKYBOX_DOWN]->num_indices = 6;
	sky_cube[SKYBOX_DOWN]->indices = new int[6];
			 			 
	sky_cube[SKYBOX_DOWN]->indices[0] = 2;
	sky_cube[SKYBOX_DOWN]->indices[1] = 1;
	sky_cube[SKYBOX_DOWN]->indices[2] = 0;
	sky_cube[SKYBOX_DOWN]->indices[3] = 2;
	sky_cube[SKYBOX_DOWN]->indices[4] = 3;
	sky_cube[SKYBOX_DOWN]->indices[5] = 1;

	// ---------------------

	sky_cube[SKYBOX_DOWN]->indices_id = sky_cube[SKYBOX_DOWN]->CreateBuffer();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sky_cube[SKYBOX_DOWN]->indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 6, sky_cube[SKYBOX_DOWN]->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// ----------------------------------------------------------------------------

	//Load The texture if it's valid 
	sky_cube[SKYBOX_DOWN]->num_uvs = sky_cube[SKYBOX_DOWN]->num_vertices;
	sky_cube[SKYBOX_DOWN]->uvs_cords = new float[sky_cube[SKYBOX_DOWN]->num_uvs * 2];
			 			 
	sky_cube[SKYBOX_DOWN]->uvs_cords[0] = 0.0f;
	sky_cube[SKYBOX_DOWN]->uvs_cords[1] = 1.0f;
			 			 
	sky_cube[SKYBOX_DOWN]->uvs_cords[2] = 1.0f;
	sky_cube[SKYBOX_DOWN]->uvs_cords[3] = 1.0f;
			 			 
	sky_cube[SKYBOX_DOWN]->uvs_cords[4] = 0.0f;
	sky_cube[SKYBOX_DOWN]->uvs_cords[5] = 0.0f;
			 			 
	sky_cube[SKYBOX_DOWN]->uvs_cords[6] = 1.0f;
	sky_cube[SKYBOX_DOWN]->uvs_cords[7] = 0.0f;

	Material* curr_mat = (Material*)App->resources->Get(RES_MATERIAL, name_from_path.c_str());

	if (curr_mat != nullptr)
	{
		curr_mat->LoadToMemory();
		sky_textures[SKYBOX_DOWN] = curr_mat->GetDiffuseTexture();
		curr_mat->reference_counting++;
	}
		
	// ---------------------

	sky_cube[SKYBOX_DOWN]->uvs_id = sky_cube[SKYBOX_DOWN]->CreateBuffer();
	glBindBuffer(GL_ARRAY_BUFFER, sky_cube[SKYBOX_DOWN]->uvs_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, sky_cube[SKYBOX_DOWN]->uvs_cords, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

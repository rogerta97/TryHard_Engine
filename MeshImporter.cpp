#include "MeshImporter.h"
#include "Globals.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}

bool MeshImporter::Start()
{
	CreatePlaneMesh(); 
	imp_type = IMP_MESH;

	return true;
}

bool MeshImporter::Update()
{

	return false;
}

bool MeshImporter::CleanUp()
{
	return false;
}

Mesh * MeshImporter::CreateCubeMesh()
{
	return nullptr;
}

Mesh * MeshImporter::CreatePlaneMesh()
{
	Mesh* mesh = new Mesh(); 
	mesh->SetPlaneData(); 
	mesh_list.push_back(mesh); 

	return mesh;
}

std::list<GameObject*> MeshImporter::CreateFBXMesh(const char* full_path)
{
	//Create the list where meshes are stored
	std::list<GameObject*> output_list; 

	//Use Assimp to load the file 
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	//If the file is loaded, go on reading the data.
	if (scene != nullptr && scene->HasMeshes())
	{
		int num_meshes = scene->mNumMeshes; 

		/*for (int i = 0; i < num_meshes; i++)
		{*/
			//Create the GameObject where the data will be stored
			GameObject* game_object = new GameObject(); 

			//Create the mesh where the data will be stored 
			aiMesh* curr_mesh = scene->mMeshes[0]; 
			Mesh* new_mesh = new Mesh(); 
			new_mesh->type = MESH_FBX; 
			new_mesh->name = curr_mesh->mName.C_Str(); 

			//Load Vertices
			new_mesh->num_vertex = curr_mesh->mNumVertices;
			new_mesh->vertex = new float3[new_mesh->num_vertex];
			memcpy(new_mesh->vertex, curr_mesh->mVertices, sizeof(float3) * new_mesh->num_vertex);

			glGenBuffers(1, &new_mesh->vertex_id); 
			glBindBuffer(GL_ARRAY_BUFFER, new_mesh->vertex_id); 
			glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*new_mesh->num_vertex, new_mesh->vertex, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0); 

			CONSOLE_LOG("%d vertices", new_mesh->num_vertex);
	
			//Load Indices
			if (curr_mesh->HasFaces())
			{
				new_mesh->num_indices = curr_mesh->mNumFaces * 3; 
				new_mesh->indices = new int[new_mesh->num_indices]; 

				for (int j = 0; j < curr_mesh->mNumFaces; j++)
				{
					aiFace curr_face = curr_mesh->mFaces[j]; 

					if (curr_face.mNumIndices != 3)
					{
						CONSOLE_ERROR("Geometry index in face %d is != 3", j); 
					}
					else
						memcpy(&new_mesh->indices[j * 3], curr_face.mIndices, sizeof(uint)* 3);					
				}

				glGenBuffers(1, &new_mesh->indices_id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->indices_id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*new_mesh->num_indices, &new_mesh->indices, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				CONSOLE_LOG("%d indices", new_mesh->num_indices); 
			}
			//Add it to Mesh List
			
			ComponentMesh* cmp_mesh = (ComponentMesh*)game_object->CreateComponent(CMP_RENDERER); 
			cmp_mesh->SetMesh(new_mesh);
			game_object->AddComponent(cmp_mesh); 

			output_list.push_back(game_object); 
	
		
		//Release Scene
		aiReleaseImport(scene);
	}
	else
	{
		CONSOLE_ERROR("Error loading scene %s", full_path);
	}


	return output_list;
}

Mesh * MeshImporter::GetMeshByType(BasicMeshType type)
{
	for (auto it = mesh_list.begin(); it != mesh_list.end(); it++)
	{
		if (type == (*it)->GetType())
		{
			return (*it);
		}
	}

	return nullptr;
}

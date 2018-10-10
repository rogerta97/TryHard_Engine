#include "MeshImporter.h"
#include "Globals.h"
#include "OpenGL.h"
#include "GameObject.h"
#include "Application.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

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
	CreateCubeMesh();
	CreateSphereMesh();
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
	Mesh* mesh = new Mesh();
	mesh->SetCubeData();
	mesh_list.push_back(mesh);

	return mesh;
}

Mesh * MeshImporter::CreatePlaneMesh()
{
	Mesh* mesh = new Mesh(); 
	mesh->SetPlaneData(); 
	mesh_list.push_back(mesh); 

	return mesh;
}

Mesh * MeshImporter::CreateSphereMesh()
{
	Mesh* mesh = new Mesh();
	mesh->SetSphereData();
	mesh_list.push_back(mesh);

	return mesh;
}

std::list<GameObject*> MeshImporter::CreateFBXMesh(const char* full_path)
{
	//Create the list where meshes are stored
	std::list<GameObject*> output_list; 

	//Use Assimp to load the file 
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	
	CONSOLE_LOG("========================");
	CONSOLE_LOG("Started loading scene from '%s'",full_path);

	//If the file is loaded, go on reading the data.
	if (scene != nullptr && scene->HasMeshes())
	{
		int num_meshes = scene->mNumMeshes; 

		CONSOLE_LOG("Scene has %d meshes", num_meshes);

		CONSOLE_LOG("");

		for (int i = 0; i < num_meshes; i++)
		{
			//Create the GameObject where the data will be stored
			GameObject* game_object = new GameObject();

			//Create the transformation. For now it will lay here. But coordinates need to be loaded from the fbx
			ComponentTransform* trans_cmp = (ComponentTransform*)game_object->CreateComponent(CMP_TRANSFORM);
			game_object->AddComponent(trans_cmp); 

			//Create the mesh where the data will be stored 
			aiMesh* curr_mesh = scene->mMeshes[i];
			Mesh* new_mesh = new Mesh();
			new_mesh->type = MESH_FBX;

			aiNode* curr_node = scene->mRootNode->mChildren[i]; 
			game_object->name = new_mesh->name = curr_node->mName.C_Str();

			string name = game_object->name;
			CONSOLE_LOG("Loaded mesh '%s':", name.c_str());

			//Load Vertices
			new_mesh->num_vertices = curr_mesh->mNumVertices;
			new_mesh->vertices = new float3[new_mesh->num_vertices];
			memcpy(new_mesh->vertices, curr_mesh->mVertices, sizeof(float3) * new_mesh->num_vertices);

			glGenBuffers(1, &new_mesh->vertices_id);
			glBindBuffer(GL_ARRAY_BUFFER, new_mesh->vertices_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*new_mesh->num_vertices, new_mesh->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			CONSOLE_LOG(" vertices: %d", new_mesh->num_vertices);

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
						memcpy(&new_mesh->indices[j * 3], curr_face.mIndices, sizeof(uint) * 3);
				}

				CONSOLE_LOG(" faces: %d", curr_mesh->mNumFaces);

				glGenBuffers(1, &new_mesh->indices_id);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->indices_id);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

				CONSOLE_LOG(" indices: %d", new_mesh->num_indices);
			}
			else
				CONSOLE_LOG("%s has no indices", name.c_str());

			//Load UV Coords
			if (curr_mesh->HasTextureCoords(0))
			{
				//Load the UV's
				new_mesh->num_uvs = new_mesh->num_indices;

				new_mesh->uvs_cords = new float[new_mesh->num_uvs*3];
				memcpy(new_mesh->uvs_cords, curr_mesh->mTextureCoords[0], sizeof(float) * new_mesh->num_uvs*3);


				glGenBuffers(1, &new_mesh->uvs_id);
				glBindBuffer(GL_ARRAY_BUFFER, new_mesh->uvs_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(uint)*new_mesh->num_uvs * 2, new_mesh->uvs_cords, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				CONSOLE_LOG(" uv's: %d", new_mesh->num_indices);
			}
			else
				CONSOLE_LOG("%s has no uv's", name.c_str());

			//Load Normals
			if (curr_mesh->HasNormals())
			{
				new_mesh->num_normals = new_mesh->num_vertices;
				new_mesh->normal_cords = new float3[new_mesh->num_normals];
				memcpy(new_mesh->normal_cords, &curr_mesh->mNormals[0], sizeof(float3) * new_mesh->num_normals);

				glGenBuffers(1, &new_mesh->normals_id);
				glBindBuffer(GL_ARRAY_BUFFER, new_mesh->normals_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*new_mesh->num_normals, new_mesh->normal_cords, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				CONSOLE_LOG(" normals: %d", new_mesh->num_normals);
			}
			else
				CONSOLE_LOG("%s has no normal cords", name.c_str());

			//Add Mesh to GameObject
			ComponentMesh* cmp_mesh = (ComponentMesh*)game_object->CreateComponent(CMP_RENDERER);
			cmp_mesh->SetMesh(new_mesh);
			game_object->AddComponent(cmp_mesh);
			cmp_mesh->CreateEnclosedMeshAABB(); 
			cmp_mesh->draw_bounding_box = false;

			if (scene->HasMaterials())
			{
				//Load Texture Image
				aiMaterial* mat = scene->mMaterials[curr_mesh->mMaterialIndex];

				//Get the path
				aiString texture_path;
				mat->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
				std::string path = App->file_system->GetModelsPath() + texture_path.C_Str();

				//Create the texture
				Texture* new_texture = new Texture();
				new_texture = App->resources->texture_importer->LoadTexture(path.c_str());

				//Create The Component
				ComponentMaterial* cmp_mat = (ComponentMaterial*)game_object->CreateComponent(CMP_MATERIAL);
				cmp_mat->diffuse = new_texture;

				//Add it to the parent GO
				game_object->AddComponent(cmp_mat);

				CONSOLE_LOG(" Component material created from texture %s.", texture_path.C_Str());
			}
			else
				CONSOLE_LOG("%s has no material", name.c_str());


			App->scene->AddGameObjectToScene(game_object); 
			output_list.push_back(game_object);

			CONSOLE_LOG("");
			CONSOLE_LOG("Succesfully created game object '%s'", name.c_str());
			CONSOLE_LOG("");
		}
		
		//Release Scene
		aiReleaseImport(scene);
	}
	else
	{
		CONSOLE_ERROR("Error loading scene %s", full_path);
	}
	CONSOLE_LOG("========================");


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


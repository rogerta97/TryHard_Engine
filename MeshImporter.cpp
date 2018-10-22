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

void LogAssimpLogs(const char * str, char * userData);

MeshImporter::MeshImporter()
{
}


MeshImporter::~MeshImporter()
{
}

bool MeshImporter::Start()
{
	struct aiLogStream logs;
	logs = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	logs.callback = LogAssimpLogs;
	aiAttachLogStream(&logs);

	CreatePlaneMesh();
	CreateCubeMesh();

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
	//mesh->SetPlaneData();
	mesh_list.push_back(mesh);

	return mesh;
}

Mesh * MeshImporter::CreateSphereMesh()
{
	Mesh* mesh = new Mesh();
	//mesh->SetSphereData();
	mesh_list.push_back(mesh);

	return mesh;
}

void MeshImporter::DrawMeshList()
{
	std::vector<string> models_files = App->file_system->GetFilesInDirectory(App->file_system->GetModelsPath().c_str());

	if (ImGui::BeginPopup("select_mesh"))
	{
		for (auto it = models_files.begin(); it != models_files.end(); it++)
		{
			if ((*it) != "." && (*it) != "..")
			{
				ImGui::Selectable((*it).c_str());
			}
		
		}
			

		ImGui::EndPopup();
	}
}

GameObject* MeshImporter::CreateFBXMesh(const char* full_path)
{
	//Use Assimp to load the file 
	GameObject* to_ret = nullptr; 
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr)
	{
		aiNode* root_node = scene->mRootNode;

		GameObject* tmp_go = new GameObject();

		LoadFBXMesh(full_path, root_node, (aiScene*)scene, tmp_go);

		to_ret = tmp_go->GetChild(0);

		delete(tmp_go);
		to_ret->parent = nullptr;

		App->scene->SetSelectedGameObject(to_ret);

		aiReleaseImport(scene);
	}
	else
	{
		CONSOLE_ERROR("FBX could not be loaded"); 
	}

	//Release Scene
	

	return to_ret;

}

void MeshImporter::LoadFBXMesh(const char * full_path, aiNode * node, aiScene * scene, GameObject* parent_gameobject)
{
	//This node contains mesh information (vertices, indices...)

	GameObject* game_object = new GameObject();

	if (node->mParent != nullptr)
		game_object->SetParent(parent_gameobject);

	if (std::string(node->mName.C_Str()) == std::string("RootNode"))
	{
		game_object->SetParent(parent_gameobject);
		game_object->name = App->file_system->GetLastPathItem(full_path, false);
	}
	else
		game_object->name = node->mName.C_Str(); 

	uint found = game_object->name.find(string("_$AssimpFbx$_")); 

	if (found != string::npos) //It has transform info
	{
		string new_name = game_object->name.substr(0, found); 
		game_object->name = new_name;
	}
	
	if(node->mTransformation[0] != nullptr)
	{
		//Create the transformation. For now it will lay here. But coordinates need to be loaded from the fbx
		ComponentTransform* trans_cmp = (ComponentTransform*)game_object->AddComponent(CMP_TRANSFORM);

		aiVector3D translation;
		aiVector3D scaling;
		aiQuaternion rotation;

		node->mTransformation.Decompose(scaling, rotation, translation);

		float3 pos(translation.x, translation.y, translation.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
		float3 esc(scaling.x, scaling.y, scaling.z);

		trans_cmp->SetPosition(pos);
		trans_cmp->SetRotation(rot); 
		trans_cmp->SetScale(esc); 

	}
		
	if (node->mNumMeshes > 0)
	{
		//Create the GameObject where the data will be stored
		int num_meshes = node->mNumMeshes;

		for (int i = 0; i < num_meshes; i++)
		{			

			CONSOLE_LOG("Creating Game Object Mesh..."); 
			//Create the mesh where the data will be stored 
			aiMesh* curr_mesh = scene->mMeshes[node->mMeshes[i]];
			Mesh* new_mesh = new Mesh();
			new_mesh->type = MESH_FBX;

			game_object->name = node->mName.C_Str(); 
			new_mesh->name = curr_mesh->mName.C_Str(); 

			//If the mesh already exist, we load it directly from library
			string mesh_lib_path = App->file_system->GetLibraryPath() + "Meshes"; 
			string file_name = game_object->name + ".mesh";

			if (App->file_system->IsFileInDirectory(mesh_lib_path.c_str(), file_name.c_str()))
			{			
				if(new_mesh->LoadFromBinary(file_name.c_str()))
					new_mesh->LoadToMemory(); 		
			}
			else
			{
				//Load Vertices
				new_mesh->num_vertices = curr_mesh->mNumVertices;
				new_mesh->vertices = new float3[new_mesh->num_vertices];
				memcpy(new_mesh->vertices, curr_mesh->mVertices, sizeof(float3) * new_mesh->num_vertices);

				glGenBuffers(1, &new_mesh->vertices_id);
				glBindBuffer(GL_ARRAY_BUFFER, new_mesh->vertices_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*new_mesh->num_vertices, new_mesh->vertices, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				CONSOLE_DEBUG("Game Object %s loaded with %d vertices", game_object->name.c_str(), new_mesh->num_vertices);

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

					glGenBuffers(1, &new_mesh->indices_id);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->indices_id);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

					CONSOLE_DEBUG("Game Object %s loaded with %d indices", game_object->name.c_str(), new_mesh->num_indices);
				}

				//Load UV Coords
				if (curr_mesh->HasTextureCoords(0))
				{
					//Load the UV's
					new_mesh->num_uvs = curr_mesh->mNumVertices;
					new_mesh->uvs_cords = new float[new_mesh->num_uvs * 3];

					if (new_mesh->uvs_cords != nullptr)
						glTexCoordPointer(2, GL_FLOAT_R_NV, 0, &new_mesh->uvs_cords);
					memcpy(new_mesh->uvs_cords, curr_mesh->mTextureCoords[0], sizeof(float) * new_mesh->num_uvs * 3);

					glGenBuffers(1, &new_mesh->uvs_id);
					glBindBuffer(GL_ARRAY_BUFFER, new_mesh->uvs_id);
					glBufferData(GL_ARRAY_BUFFER, sizeof(float)*new_mesh->num_uvs * 3, new_mesh->uvs_cords, GL_STATIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					CONSOLE_DEBUG("Game Object %s loaded with %d UV's", game_object->name.c_str(), new_mesh->num_uvs);
				}

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

					CONSOLE_DEBUG("Game Object %s loaded with %d normals", game_object->name.c_str(), new_mesh->num_normals);
				}

				new_mesh->SaveAsBinary(game_object->name.c_str()); 		
			}
				
			//Add Mesh to GameObject
			ComponentMesh* cmp_mesh = (ComponentMesh*)game_object->AddComponent(CMP_RENDERER);
			cmp_mesh->SetMesh(new_mesh);
			cmp_mesh->CreateEnclosedMeshAABB();
			cmp_mesh->draw_bounding_box = false;

			if (scene->HasMaterials())
			{
				CONSOLE_LOG("Loading Texture attached to %s", new_mesh->name.c_str());
			
				//Load Texture Image
				aiMaterial* mat = nullptr;
				mat = scene->mMaterials[curr_mesh->mMaterialIndex];

				//Get the path
				aiString texture_name;
				mat->GetTexture(aiTextureType_DIFFUSE, 0, &texture_name);

				if (string(texture_name.C_Str()) != string(""))
				{
					CONSOLE_LOG("Texture attached: %s", texture_name.C_Str());

					std::string path = App->file_system->GetTexturesPath() + texture_name.C_Str();

					//Create the texture
					Texture* new_texture = new Texture(); 
					new_texture = App->resources->texture_importer->LoadTexture(path.c_str());

					if (new_texture != nullptr)
					{
						CONSOLE_LOG("Texture Loaded Succesfully from: %s", path.c_str());
					}

					//Create The Component
					ComponentMaterial* cmp_mat = (ComponentMaterial*)game_object->AddComponent(CMP_MATERIAL);
					cmp_mat->diffuse = new_texture;

					//Add it to the parent GO
					
				}
				else
				{
					CONSOLE_ERROR("Texture not bounded correctly to '%s' Mesh, Texture won't be applied", game_object->name.c_str());
					return; 
				}

				App->scene->AddGameObjectToScene(game_object);
			}
		}
	}
	else //The node contains other type of information (transform, light?)
	{
		App->scene->AddGameObjectToScene(game_object);
	}


	if (node->mNumChildren > 0)
	{
		for (int i = 0; i < node->mNumChildren; i++)
		{
			LoadFBXMesh(full_path, node->mChildren[i], scene, game_object);
		}
	}

	parent_gameobject = game_object;
}

Mesh* MeshImporter::GetMeshByType(BasicMeshType type)
{
	Mesh* new_ret_mesh = new Mesh(); 

	for (auto it = mesh_list.begin(); it != mesh_list.end(); it++)
	{
		if (type == (*it)->GetType())
		{
			memcpy(new_ret_mesh, (*it), sizeof(Mesh)); 
		}
	}

	return new_ret_mesh;
}

void LogAssimpLogs(const char * str, char * userData)
{
	CONSOLE_LOG("%s", str);
}
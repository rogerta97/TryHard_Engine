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

#include "MaterialImporter.h"

#include <string>
#include <fstream>

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

	//CreatePlaneMesh();
	//CreateCubeMesh();

	ImportAllFilesFromAssets(); 

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

	return mesh;
}

Mesh * MeshImporter::CreatePlaneMesh()
{
	Mesh* mesh = new Mesh();
	//mesh->SetPlaneData();

	return mesh;
}

Mesh * MeshImporter::CreateSphereMesh()
{
	Mesh* mesh = new Mesh();
	//mesh->SetSphereData();

	return mesh;
}

void MeshImporter::ImportAllFilesFromAssets()
{
	vector<string> files = App->file_system->GetAllFilesInDirectory(App->file_system->GetModelsPath().c_str(), true);

	for (auto it = files.begin(); it != files.end(); it++)
	{
		GameObject* es_locura = CreateFBXMesh((*it).c_str());
		es_locura->DeleteRecursive();
	}
}

void MeshImporter::DrawMeshList()
{
	std::vector<string> models_files = App->file_system->GetAllFilesInDirectory(App->file_system->GetModelsPath().c_str(), false);

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
	bool load_succes = true;

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
	
	ComponentTransform* trans_cmp = nullptr; 

	if(node->mTransformation[0] != nullptr)
	{
		//Create the transformation. For now it will lay here. But coordinates need to be loaded from the fbx

		aiVector3D translation;
		aiVector3D scaling;
		aiQuaternion rotation;

		node->mTransformation.Decompose(scaling, rotation, translation);

		float3 pos(translation.x, translation.y, translation.z);
		Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
		float3 esc(scaling.x, scaling.y, scaling.z);

		ComponentTransform* trans_cmp = (ComponentTransform*)game_object->GetComponent(CMP_TRANSFORM);

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
			Mesh* new_mesh = nullptr;
		//	new_mesh->type = MESH_FBX;

			string tmp_name = node->mName.C_Str(); 
			game_object->name = tmp_name;

			//If the mesh already exist, we load it directly from library
			string mesh_lib_path = App->file_system->GetLibraryPath() + string("\\") + "Meshes";
			string file_name = tmp_name + ".mesh";

			if (App->file_system->IsFileInDirectory(mesh_lib_path.c_str(), file_name.c_str()))
			{						
				new_mesh = App->resources->mesh_importer->LoadFromBinary(file_name.c_str());
				new_mesh->name = game_object->name;
				new_mesh->type = MESH_FBX;
				new_mesh->LoadToMemory();
			}
			else
			{
				//Create the resource 
				new_mesh = (Mesh*)App->resources->CreateNewResource(RES_MESH); 

				new_mesh->name = game_object->name + ".mesh";
				new_mesh->path = mesh_lib_path + "\\" + game_object->name;

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
							load_succes = false;
							break;
						}
						else
							memcpy(&new_mesh->indices[j * 3], curr_face.mIndices, sizeof(uint) * 3);
					}

					if (load_succes) {
						glGenBuffers(1, &new_mesh->indices_id);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->indices_id);
						glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)*new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

						CONSOLE_DEBUG("Game Object %s loaded with %d indices", game_object->name.c_str(), new_mesh->num_indices);
					}
					else
					{
						CONSOLE_ERROR("Component mesh of %s not created", game_object->name.c_str(), new_mesh->num_normals);
						game_object->DeleteRecursive();		
						App->resources->AddResourceToDelete(new_mesh->GetUID()); 
						new_mesh->~Mesh();
						break;
					}
				}

				if (load_succes)
				{
					//Load UV Coords
					if (curr_mesh->HasTextureCoords(0))
					{
						//Load the UV's
						new_mesh->num_uvs = curr_mesh->mNumVertices;
						new_mesh->uvs_cords = new float[new_mesh->num_uvs * 3];

						if (new_mesh->uvs_cords != nullptr)
							glTexCoordPointer(3, GL_FLOAT_R_NV, 0, &new_mesh->uvs_cords);

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

					//Create the mesh resource
					App->resources->mesh_importer->Import((Mesh*)new_mesh, game_object->name.c_str());					
				}
				
			}


			//Add Mesh to GameObject
			ComponentMesh* cmp_mesh = (ComponentMesh*)game_object->AddComponent(CMP_MESH);
			cmp_mesh->SetMesh(new_mesh);
			cmp_mesh->CreateEnclosedMeshAABB();
			cmp_mesh->UpdateBoundingBox();
			cmp_mesh->draw_bounding_box = false;
			cmp_mesh->container_fbx = game_object->GetRootParent()->GetChild(0)->name;


			if (scene->HasMaterials())
			{
				CONSOLE_LOG("Loading Texture attached to %s", new_mesh->name.c_str());
			
				//Load Texture Image
				aiMaterial* mat = nullptr;
				mat = scene->mMaterials[curr_mesh->mMaterialIndex];

				//Get the path
				aiString texture_name;
				mat->GetTexture(aiTextureType_DIFFUSE, 0, &texture_name);

				string folder_to_check = App->file_system->GetLibraryPath() + std::string("\\") + "Materials"; 

				string item_ass_name = App->file_system->GetLastPathItem(texture_name.C_Str(), true);
				string item_lib_name = App->file_system->GetLastPathItem(texture_name.C_Str()) + ".dds";

				if(string(texture_name.C_Str()) != string(""))
				{
					Material* new_mat = nullptr; 

					if (App->file_system->IsFileInDirectory(folder_to_check.c_str(), item_lib_name.c_str()))
					{
						string path = folder_to_check + string("\\") + item_lib_name;

						new_mat = App->resources->material_importer->LoadFromBinary(path.c_str());

						new_mat->SetType(resource_type::RES_MATERIAL);
					}
					else
					{

						//The program will never enter here, every texture dragged or in folder will be generated as resource.

						//new_mat = new Material(); 
						//CONSOLE_LOG("Texture attached: %s", texture_name.C_Str());

						//std::string path = App->file_system->GetTexturesPath() + string("\\") + texture_name.C_Str();

						////Create the texture
						//Texture* new_texture = new Texture();
						//new_texture = App->resources->material_importer->LoadTexture(path.c_str());

						//if (new_texture != nullptr)
						//{
						//	new_mat->SetDiffuseTexture(new_texture); 
						//	CONSOLE_LOG("Texture Loaded Succesfully from: %s", path.c_str());
						//}		

						//Material* new_mat_res = (Material*)App->resources->CreateNewResource(RES_MATERIAL);
						//new_mat_res->SetDiffuseTexture(new_texture);

						//new_mat_res->path = path.c_str();
						//new_mat_res->name = App->file_system->GetLastPathItem(texture_name.C_Str(), true);

						//App->resources->material_importer->Import(new_mat_res, texture_name.C_Str());
					}

					//Create The Component
					ComponentMaterial* cmp_mat = (ComponentMaterial*)game_object->AddComponent(CMP_MATERIAL);
					cmp_mat->SetMaterial(new_mat);
				}
				else
				{
					aiColor3D mat_color(1.0f, 1.0f, 1.0f);
					if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, mat_color) == aiReturn_SUCCESS)
					{
						ComponentMaterial* cmp_mat = (ComponentMaterial*)game_object->AddComponent(CMP_MATERIAL);
						cmp_mat->SetColor({ mat_color.r,mat_color.g,mat_color.b });
						CONSOLE_DEBUG("'%s' Has no texture but a component material will be created for the color", game_object->name.c_str());
					}
					else 
						CONSOLE_ERROR("Texture or color not bounded correctly to '%s' Mesh, Component material won't be applied", game_object->name.c_str());
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

bool MeshImporter::Import(Mesh * saving_mesh, const char * mesh_name)
{
	CONSOLE_DEBUG("Mesh '%s' was not found in library. Saving to binary....", mesh_name);

	string save_path = App->file_system->GetLibraryPath() + '\\' + "Meshes\\" + mesh_name + ".mesh";

	if (saving_mesh->num_vertices == 0)
		return false;

	//Create a new file or open it 
	std::ofstream stream;
	stream.open(save_path, std::fstream::binary | std::fstream::out);
	stream.clear();

	//Create the buffer and allocate the space. Data will be stored Ranges - Vertices - Indices - UVS - Normals
	uint size_of_data = (sizeof(uint) * 4) + (sizeof(float)*saving_mesh->num_vertices * 3) + (sizeof(uint)*saving_mesh->num_indices) + (sizeof(float)*saving_mesh->num_uvs * 3) + (sizeof(float)*saving_mesh->num_normals * 3);
	GLubyte* buffer = new GLubyte[size_of_data];
	GLubyte* cursor = buffer;

	//Allocate Ranges
	uint ranges[4] = { saving_mesh->num_vertices , saving_mesh->num_indices , saving_mesh->num_uvs , saving_mesh->num_normals };
	uint bytes = sizeof(ranges);
	memcpy(cursor, ranges, bytes);

	//Allocate Vertices 
	cursor += bytes;
	bytes = sizeof(float)*saving_mesh->num_vertices * 3;
	memcpy(cursor, saving_mesh->vertices, bytes);

	//Allocate Indices
	cursor += bytes;
	bytes = sizeof(uint)*saving_mesh->num_indices;
	memcpy(cursor, saving_mesh->indices, bytes);

	//Allocate UVS
	cursor += bytes;
	bytes = sizeof(float)*saving_mesh->num_uvs * 3;
	memcpy(cursor, saving_mesh->uvs_cords, bytes);

	//Allocate Normals
	cursor += bytes;
	bytes = sizeof(float)*saving_mesh->num_normals * 3;
	memcpy(cursor, saving_mesh->normal_cords, bytes);

	//Save data to the file
	stream.write((const char*)buffer, size_of_data);
	stream.close();

	return true;
}

Mesh * MeshImporter::LoadFromBinary(const char * mesh_name)
{
	Mesh* mesh_to_ret = (Mesh*)App->resources->CreateNewResource(RES_MESH);;

	CONSOLE_DEBUG("Mesh '%s' has been FOUND in library. Loading mesh...", mesh_name);

	string mesh_path = App->file_system->GetLibraryPath() + '\\' + "Meshes\\" + mesh_name;

	//Open the file for reading
	std::ifstream stream;
	stream.open(mesh_path, std::fstream::binary);

	if (stream)
	{
		// Get length of file:
		stream.seekg(0, stream.end);
		int length = stream.tellg();
		stream.seekg(0, stream.beg);

		// Create the buffer where the data is going to be stored
		char* buffer = new char[length];
		stream.read(buffer, sizeof(char) * length);
		char* cursor = buffer;

		////Get Ranges
		uint ranges[4];
		uint bytes = sizeof(uint) * 4;
		memcpy(ranges, cursor, bytes);
		cursor += bytes;

		mesh_to_ret->num_vertices = ranges[0];
		mesh_to_ret->num_indices = ranges[1];
		mesh_to_ret->num_uvs = ranges[2];
		mesh_to_ret->num_normals = ranges[3];

		//Get Vertices 		
		bytes = sizeof(float)*mesh_to_ret->num_vertices * 3;
		mesh_to_ret->vertices = new float3[mesh_to_ret->num_vertices];
		memcpy(mesh_to_ret->vertices, cursor, bytes);
		cursor += bytes;

		//Get Indices	
		bytes = sizeof(uint)*mesh_to_ret->num_indices;
		mesh_to_ret->indices = new int[mesh_to_ret->num_indices];
		memcpy(mesh_to_ret->indices, cursor, bytes);

		//Get UVS		
		if (mesh_to_ret->num_uvs != 0)
		{
			cursor += bytes;
			bytes = sizeof(float)*mesh_to_ret->num_uvs * 3;
			mesh_to_ret->uvs_cords = new float[mesh_to_ret->num_uvs * 3];
			memcpy(mesh_to_ret->uvs_cords, cursor, bytes);
		}

		//Get Normals
		if (mesh_to_ret->num_normals)
		{
			cursor += bytes;
			bytes = sizeof(float)*mesh_to_ret->num_normals * 3;
			mesh_to_ret->normal_cords = new float3[mesh_to_ret->num_normals];
			memcpy(mesh_to_ret->normal_cords, cursor, bytes);
		}
	}

	//Close the file	
	stream.close();

	return mesh_to_ret;
}

void LogAssimpLogs(const char * str, char * userData)
{
	CONSOLE_LOG("%s", str);
}
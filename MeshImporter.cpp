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
	return true;

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

GameObject* MeshImporter::CreateFBXMesh(const char* full_path)
{
	//Use Assimp to load the file 
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	aiNode* root_node = scene->mRootNode;

	GameObject* tmp_go = new GameObject();

	LoadFBXMesh(full_path, root_node, (aiScene*)scene, tmp_go);

	//Release Scene
	aiReleaseImport(scene);

	GameObject* to_ret = tmp_go->GetChild(0);

	delete(tmp_go);
	to_ret->parent = nullptr; 

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
		ComponentTransform* trans_cmp = (ComponentTransform*)game_object->CreateComponent(CMP_TRANSFORM);

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

		game_object->AddComponent(trans_cmp);
	}
		
	if (node->mNumMeshes > 0)
	{
		//Create the GameObject where the data will be stored
		int num_meshes = node->mNumMeshes;

		for (int i = 0; i < num_meshes; i++)
		{			
			//Create the mesh where the data will be stored 
			aiMesh* curr_mesh = scene->mMeshes[node->mMeshes[i]];
			Mesh* new_mesh = new Mesh();
			new_mesh->type = MESH_FBX;

			game_object->name = node->mName.C_Str(); 

			//Load Vertices
			new_mesh->num_vertices = curr_mesh->mNumVertices;
			new_mesh->vertices = new float3[new_mesh->num_vertices];
			memcpy(new_mesh->vertices, curr_mesh->mVertices, sizeof(float3) * new_mesh->num_vertices);

			glGenBuffers(1, &new_mesh->vertices_id);
			glBindBuffer(GL_ARRAY_BUFFER, new_mesh->vertices_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*new_mesh->num_vertices, new_mesh->vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			CONSOLE_LOG("%d vertices", new_mesh->num_vertices);

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

				CONSOLE_LOG("%d indices", new_mesh->num_indices);
			}

			//Load UV Coords
			if (curr_mesh->HasTextureCoords(0))
			{
				//Load the UV's
				new_mesh->num_uvs = new_mesh->num_indices;
				new_mesh->uvs_cords = new float[new_mesh->num_uvs * 3];			
				memcpy(new_mesh->uvs_cords, curr_mesh->mTextureCoords[0], sizeof(float) * new_mesh->num_uvs * 3);

				glGenBuffers(1, &new_mesh->uvs_id);
				glBindBuffer(GL_ARRAY_BUFFER, new_mesh->uvs_id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float)*new_mesh->num_uvs * 3, new_mesh->uvs_cords, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
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
			}

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

void LogAssimpLogs(const char * str, char * userData)
{
	CONSOLE_LOG("%s", str);
}
#include "MaterialImporter.h"
#include "Application.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "ComponentMaterial.h"


MaterialImporter::MaterialImporter()
{

}

bool MaterialImporter::Start()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	string symbol_path = "";

	//Create GO Icon Resource
	symbol_path = string(App->file_system->GetTexturesPath() + string("\\") + "EngineTextures\\GameObjectIcon.png");
	LoadTexture(symbol_path.c_str(), true);

	//Create Folder Icon Resource
	symbol_path = string(App->file_system->GetTexturesPath() + string("\\") + "EngineTextures\\FolderIcon.png");
	LoadTexture(symbol_path.c_str(), true);

	//Create Mesh Icon Resource
	symbol_path = string(App->file_system->GetTexturesPath() + string("\\") + "EngineTextures\\MeshIcon.png");
	LoadTexture(symbol_path.c_str(), true);

	//Create Image Icon Resource
	symbol_path = string(App->file_system->GetTexturesPath() + string("\\") + "EngineTextures\\ImageIcon.png");
	LoadTexture(symbol_path.c_str(), true);

	//Create Font Icon Resource
	symbol_path = string(App->file_system->GetTexturesPath() + string("\\") + "EngineTextures\\FontIcon.png");
	LoadTexture(symbol_path.c_str(), true);

	//Create DDS Icon Resource
	symbol_path = string(App->file_system->GetTexturesPath() + string("\\") + "EngineTextures\\DDSIcon.jpg");
	LoadTexture(symbol_path.c_str(), true);

	//Create TGA Icon Resource
	symbol_path = string(App->file_system->GetTexturesPath() + string("\\") + "EngineTextures\\TGAIcon.png");
	LoadTexture(symbol_path.c_str(), true);

	//Create Scene Icon Resource
	symbol_path = string(App->file_system->GetTexturesPath() + string("\\") + "EngineTextures\\SceneIcon.png");
	LoadTexture(symbol_path.c_str(), true);

	checker_texture = new Texture();
	checker_texture->FillCheckerTextureData();

	return true;
}

bool MaterialImporter::Update()
{
	return true;
}

bool MaterialImporter::CleanUp()
{
	return true;
}

Texture* MaterialImporter::LoadTexture(const char * path, bool not_flip)
{
	Texture* tex = nullptr;

	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	//Check if the texture existed before based on the name on its path
	string new_name = App->file_system->GetLastPathItem(path).c_str();

	Resource* new_res = App->resources->Get(RES_MATERIAL, new_name.c_str()); 

	if (new_res)
		return (Texture*)new_res; 
	
	//If we already have it, no need to load
	if (tex != nullptr)
		return tex;

	success = ilLoadImage(path);

	if (success)
	{
		//Create Texture where data will be stored 
		tex = new Texture();

		//Get data of the image
		ILinfo image_info;
		iluGetImageInfo(&image_info);

		if (image_info.Origin == IL_ORIGIN_UPPER_LEFT && not_flip == false)
			iluFlipImage();

		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success)
		{
			tex->CreateBuffer();
			tex->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
			tex->SetHeight(ilGetInteger(IL_IMAGE_HEIGHT));
			tex->SetPath(path);

			tex->Bind();
			tex->SetTextureSettings();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->GetWidth(), tex->GetHeight(), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			tex->UnBind();

			//Create the texture resource			
			Material* new_mat_res = (Material*)App->resources->CreateNewResource(RES_MATERIAL);
			new_mat_res->SetDiffuseTexture(tex);
			new_mat_res->name = new_name.c_str();

			App->resources->material_importer->Import(new_mat_res, new_mat_res->name.c_str());
		}
	}

	return tex;
}

bool MaterialImporter::SaveTexturesAsDDS()
{
	//for (auto it = textures_list.begin(); it != textures_list.end(); it++)
	//{
	//	SaveTexture((*it), IL_DDS); 
	//}

	return true;
}

bool MaterialImporter::SaveTexture(Texture * tex_to_save, ILenum format_type)
{
	////Textures will be saved into DDS for the moment
	//string textures_dir = App->file_system->GetLibraryPath() + "Textures\\";

	////First, we should check if the texture has been already saved in DDS format. (TODO: resource manager) For now we will always save a new one. 
	//if (tex_to_save != nullptr)
	//{
	//	switch (format_type)
	//	{
	//	case IL_DDS:
	//	{
	//		//Save The texture
	//		{
	//			for (auto it = textures_list.begin(); it != textures_list.end(); it++)
	//			{
	//				//If the texture already exist in library, skip the saving process. 
	//				if (App->file_system->IsFileInDirectory(textures_dir.c_str(), (*it)->GetName().c_str()))
	//					continue;

	//				//If not, save it
	//				int size = ilSaveL(IL_DDS, NULL, 0);
	//				if (size > 0)
	//				{

	//					//Copy data
	//					GLubyte* data = new GLubyte[size];

	//					(*it)->Bind();
	//					data = ilGetData();

	//					if (ilSaveL(IL_DDS, data, size))
	//					{
	//						//Create the file 
	//						FILE* new_file;
	//						string save_dir = textures_dir + (*it)->GetName() + ".dds";
	//						new_file = fopen(save_dir.c_str(), "w");

	//						//Save the info 
	//						if (new_file != nullptr)
	//						{
	//							int result = fwrite(data, sizeof(ILubyte), sizeof(data), new_file);
	//						}

	//						fclose(new_file);
	//					}


	//				}
	//			}
	//		}
	//		break;
	//	}
	//	}
	//}


	return true;
}

void MaterialImporter::DeleteTextureFromList(Texture * to_del)
{
	/*for (auto it = textures_list.begin(); it != textures_list.end();)
	{
		if ((*it) == to_del)
		{
			it = textures_list.erase(it);
			continue; 
		}

		it++;
	}*/
}

bool MaterialImporter::DrawTextureList()
{
	/*static bool show_browser = false;

	ImGui::SameLine();
	if (ImGui::BeginPopup("select_texture"))
	{
		ImGui::Text("Loaded textures:");
		ImGui::Separator();
		int i = 0;

		std::vector<string> textures_on_folder = App->file_system->GetFilesInDirectory(App->file_system->GetTexturesPath().c_str());

		for (auto it = textures_on_folder.begin(); it != textures_on_folder.end(); it++, i++) {
			if ((*it) != "." && (*it) != "..")
			{
				if (ImGui::Selectable(textures_on_folder[i].c_str()))
				{
					ComponentMaterial* mat = (ComponentMaterial*)App->scene->GetSelectedGameObject()->GetComponent(CMP_MATERIAL);

					if (mat != nullptr)
					{
						string file_path = App->file_system->GetTexturesPath() + string("\\") + (*it);
						mat->GetMaterial()->SetDiffuseTexture(LoadTexture(file_path.c_str()));
					}
				}
			}
		}
		ImGui::EndPopup();
	}*/

	return false;
}


//Texture * MaterialImporter::GetTexture(const char* name)
//{
//	Texture* to_ret = new Texture(); 
//
//	for (auto it = textures_list.begin(); it != textures_list.end(); it++)
//	{
//		if ((*it)->GetName() == string(name))
//		{
//			memcpy(to_ret, (*it), sizeof(Texture));
//			return to_ret;
//		}
//			
//	}
//
//	return nullptr;
//}

void MaterialImporter::GenerateCheckerTexture()
{
	checker_texture->FillCheckerTextureData();
}

Texture * MaterialImporter::GetCheckerTexture() const
{
	return checker_texture;
}

bool MaterialImporter::Import(Material * mat_to_save, const char * tex_name)
{
	bool ret = false; 

	//Get the path to save 
	string path_to_save = App->file_system->GetLibraryPath() + '\\' + "Materials\\" + tex_name; 

	if (mat_to_save != nullptr)
	{
		//Create or open the file
		ofstream stream;
		stream.open(path_to_save, ofstream::binary);
		stream.clear(); 

		//Check the state of the Material to know what we should write (tex channels)
		//Save the diffuse channel 
		ILuint size;
		ILubyte *data;
		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5); // To pick a specific DXT compression use

		size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

		if (size > 0) {

			data = new ILubyte[size]; // allocate data buffer

			if (mat_to_save->GetDiffuseTexture() != nullptr)
			{
				mat_to_save->GetDiffuseTexture()->Bind();

				if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
				{
					ret = true;
					stream.write((const char*)data, size);
				}
			}	
		}

		stream.close(); 

		CONSOLE_DEBUG("Material '%s' saved to library", tex_name);
	}	

	return ret;
}

Material * MaterialImporter::LoadFromBinary(const char * tex_name)
{
	Material* new_mat = new Material(); 

	string tex_path = App->file_system->GetLibraryPath() + std::string("\\") + "Materials\\" + tex_name; 

	Texture* new_tex = LoadTexture(tex_path.c_str(), true);
	new_mat->SetDiffuseTexture(new_tex); 

	CONSOLE_DEBUG("Material '%s' loaded correctly from libary", tex_name); 

	return new_mat;
}

MaterialImporter::~MaterialImporter()
{
}

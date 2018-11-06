#include "MaterialImporter.h"
#include "Application.h"

#include "ModuleResources.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "ComponentMaterial.h"

#include "ModuleFileSystem.h"


MaterialImporter::MaterialImporter()
{

}

bool MaterialImporter::Start()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	ImportAllFilesFromAssets();

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
		}
	}

	return tex;
}


void MaterialImporter::ImportAllFilesFromAssets()
{
	std::vector<string> files = App->file_system->GetAllFilesInDirectory(App->file_system->GetTexturesPath().c_str(), true); 

	for (auto it = files.begin(); it != files.end(); it++)
	{	
		string lib_path = App->file_system->GetLibraryPath() + string("\\Materials"); 
		string name = App->file_system->GetLastPathItem((*it).c_str(), false);
		name += ".dds";

		Material* new_mat = nullptr; 

		if (App->file_system->IsFileInDirectory(lib_path.c_str(), name.c_str()))
		{
			string path_to_load = lib_path + string("\\") + name;
			new_mat = App->resources->material_importer->LoadFromBinary(path_to_load.c_str());

			new_mat->path = (*it).c_str();
			new_mat->name = App->file_system->GetLastPathItem((*it).c_str(), true);
			
		}
		else
		{
			new_mat = (Material*)App->resources->CreateNewResource(RES_MATERIAL);	
			Texture* tex = App->resources->material_importer->LoadTexture((*it).c_str(), true);

			new_mat->path = (*it).c_str();
			new_mat->name = App->file_system->GetLastPathItem((*it).c_str(), true);

			if (tex)
				new_mat->SetDiffuseTexture(tex);

			App->resources->material_importer->Import(new_mat, new_mat->name.c_str());

		
		}	
	}
}

bool MaterialImporter::DrawTextureList()
{
	static bool show_browser = false;

	ImGui::SameLine();
	if (ImGui::BeginPopup("select_texture"))
	{
		ImGui::Text("Loaded textures:");
		ImGui::Separator();
		int i = 0;

		vector<string> textures_on_folder = App->file_system->GetAllFilesInDirectory(App->file_system->GetTexturesPath().c_str(), false);

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
	}

	return false;
}


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
	string tex_name_alone = App->file_system->GetLastPathItem(tex_name, false); 
	string path_to_save = App->file_system->GetLibraryPath() + '\\' + "Materials\\" + tex_name_alone + ".dds";

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

Material * MaterialImporter::LoadFromBinary(const char * tex_path)
{
	Material* new_mat = (Material*)App->resources->CreateNewResource(RES_MATERIAL);

	//string tex_path = App->file_system->GetLibraryPath() + std::string("\\") + "Materials\\" + tex_name; 

	Texture* new_tex = LoadTexture(tex_path, true);
	new_mat->SetDiffuseTexture(new_tex); 

	CONSOLE_DEBUG("Material '%s' loaded correctly from libary", App->file_system->GetLastPathItem(tex_path, true).c_str());

	return new_mat;
}

MaterialImporter::~MaterialImporter()
{
}

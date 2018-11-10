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

	checker_texture = new Texture();
	checker_texture->FillCheckerTextureData();

	ImportAllFilesFromAssets();

	Material* mat = (Material*)App->resources->Get(RES_MATERIAL, "PlayIcon");



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

Texture* MaterialImporter::LoadTexture(const char * path, bool flip)
{
	Texture* new_tex = new Texture(); 

	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	//Check if the texture existed before based on the name on its path
	string new_name = App->file_system->GetLastPathItem(path, true).c_str();

	success = ilLoadImage(path);

	if (success)
	{
		//Get data of the image
		ILinfo image_info;
		iluGetImageInfo(&image_info);

		if (flip == true)
		{
			if (image_info.Origin == IL_ORIGIN_UPPER_LEFT)
				iluFlipImage();
		}
	

		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success)
		{
			new_tex->SetTextureID(new_tex->CreateBuffer());
			new_tex->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
			new_tex->SetHeight(ilGetInteger(IL_IMAGE_HEIGHT));

			new_tex->SetPath(path);
			new_tex->name = new_name;

			new_tex->Bind();
			new_tex->SetTextureSettings();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, new_tex->GetWidth(), new_tex->GetHeight(), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			new_tex->UnBind();
		}
	}

	return new_tex;
}

void MaterialImporter::FlipTexture(Texture * tex)
{
	if (tex)
	{
		tex->Bind();
		iluFlipImage();
		tex->UnBind();
	}
}


void MaterialImporter::ImportAllFilesFromAssets()
{
	std::vector<string> files = App->file_system->GetAllFilesInDirectory(App->file_system->GetTexturesPath().c_str(), true); 

	for (auto it = files.begin(); it != files.end(); it++)
	{	
		string lib_path = App->file_system->GetLibraryPath() + string("\\Materials"); 

		string ass_tex_name = App->file_system->GetLastPathItem((*it).c_str(), true); 
		string lib_tex_name = App->file_system->GetLastPathItem((*it).c_str(), false) + ".dds";

		Material* new_mat = nullptr; 

		if (!App->file_system->IsFileInDirectory(lib_path.c_str(), lib_tex_name.c_str()))
		{
			new_mat = new Material();
			new_mat->path = lib_path;
			new_mat->name = ass_tex_name;

			Texture* tex = nullptr;
			tex = App->resources->material_importer->LoadTexture((*it).c_str(), false);

			if (tex)
				new_mat->SetDiffuseTexture(tex);

			App->resources->material_importer->Import(new_mat, new_mat->name.c_str());

			Material* new_mat_res = (Material*)App->resources->CreateNewResource(RES_MATERIAL);
			new_mat_res = new_mat; 

			App->resources->material_importer->FlipTexture(new_mat->diffuse);
		}

		string path_to_load = lib_path + string("\\") + lib_tex_name;
		new_mat = App->resources->material_importer->LoadFromBinary(path_to_load.c_str());
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

		vector<string> textures_on_folder = App->file_system->GetAllFilesInDirectory(string(App->file_system->GetLibraryPath() + "\\Materials").c_str(), false);

		for (auto it = textures_on_folder.begin(); it != textures_on_folder.end(); it++, i++) {
			if ((*it) != "." && (*it) != "..")
			{
				if (ImGui::Selectable(textures_on_folder[i].c_str()))
				{
					ComponentMaterial* mat = (ComponentMaterial*)App->scene->GetSelectedGameObject()->GetComponent(CMP_MATERIAL);

					if (mat != nullptr)
					{
						Material* new_mat = (Material*)App->resources->Get(RES_MATERIAL, App->file_system->DeleteFileExtension((*it).c_str()).c_str());
						mat->SetMaterial(new_mat);
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
	file_extension ext = App->file_system->GetFileExtension(tex_name);
	string tex_name_alone = App->file_system->GetLastPathItem(tex_name, false);

	string path_dst = App->file_system->GetAssetsPath() + '\\' + "Textures\\" + tex_name; 
	string path_to_save = App->file_system->GetLibraryPath() + '\\' + "Materials\\" + tex_name_alone + ".dds";

	if (ext == FX_DDS && mat_to_save != nullptr) //if the texture is already in dds we don't need to get the data and save it in dds format, we just copy the file
	{
		std::ifstream in; 
		in.open(path_dst.c_str(), ifstream::binary);

		std::ofstream out;            
		out.open(path_to_save.c_str(), ifstream::binary);
										  
		char buf[4096];

		do {
			in.read(&buf[0], 4096);    
			out.write(&buf[0], in.gcount()); 
		} while (in.gcount() > 0);         
											
		in.close();
		out.close();

		return true;
	}
	
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
	Material* to_ret = nullptr; 

	string name = App->file_system->GetLastPathItem(tex_path);	
	string path_to_load = App->file_system->GetLibraryPath() + string("\\Materials\\") + name + ".dds";

	to_ret = (Material*)App->resources->CreateNewResource(RES_MATERIAL);

	if (to_ret)
	{
		to_ret->path = path_to_load;
		to_ret->name = name;

		Texture* new_tex = LoadTexture(path_to_load.c_str(), true);
		to_ret->SetDiffuseTexture(new_tex);
	}
	
	CONSOLE_DEBUG("Material '%s' loaded correctly from libary", App->file_system->GetLastPathItem(tex_path, true).c_str());

	return to_ret;
}

MaterialImporter::~MaterialImporter()
{
}

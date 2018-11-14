

#include "MaterialImporter.h"
#include "Application.h"

#include "ModuleResources.h"

#include "Resource.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "ComponentMaterial.h"
#include "ModuleFileSystem.h"

#include "mmgr\mmgr.h"

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
	//Get Assets Files
	std::vector<string> files = App->file_system->GetAllFilesInDirectory(App->file_system->GetTexturesPath().c_str(), true);

	Material* new_mat = nullptr; 

	for (auto it = files.begin(); it != files.end(); it++)
	{
		new_mat = (Material*)App->resources->CreateNewResource(RES_MATERIAL);

		//If the file doesn't have ".meta", we assume there is any binary in the library
		string item_directory = App->file_system->DeleteLastPathItem((*it).c_str());
		string item_meta_name = App->file_system->GetLastPathItem((*it).c_str(), true) + ".meta";

		if (!App->file_system->IsFileInDirectory(item_directory.c_str(), item_meta_name.c_str()))
		{
			string item_meta_path = (*it) + ".meta"; 

			//If the meta doesn't exist we create it. 
			std::ofstream stream;
			stream.open(item_meta_path, std::fstream::out);

			JSON_Value* scene_v = json_value_init_object();
			JSON_Object* scene_obj = json_value_get_object(scene_v);

			//Save Meta Info
			json_object_dotset_number(scene_obj, "MetaInfo.UID", new_mat->GetUID());

			json_serialize_to_file(scene_v, item_meta_path.c_str());

			SetFileAttributes(item_meta_path.c_str(), FILE_ATTRIBUTE_HIDDEN);

			Texture* tex = nullptr;
			tex = App->resources->material_importer->LoadTexture((*it).c_str(), false);

			if (tex)
				new_mat->SetDiffuseTexture(tex);

			string ass_name = App->file_system->GetLastPathItem((*it).c_str(), true);
			App->resources->material_importer->Import(new_mat, ass_name.c_str(), new_mat->GetUID());
		}

		App->resources->material_importer->LoadFromBinary((*it).c_str(), new_mat);

		new_mat->name = App->file_system->GetLastPathItem((*it).c_str());
		new_mat->path = App->file_system->GetLibraryPath() + "\\Materials\\" + to_string(new_mat->GetUID()) + ".dds";

		new_mat->UnloadFromMemory();
	}

	//string lib_path = App->file_system->GetLibraryPath() + string("\\Materials");
	//
	//string ass_tex_name = App->file_system->GetLastPathItem((*it).c_str(), true);
	//string lib_tex_name = App->file_system->GetLastPathItem((*it).c_str(), false) + ".dds";
	//
	//Material* new_mat = nullptr;
	//
	//if (!App->file_system->IsFileInDirectory(lib_path.c_str(), lib_tex_name.c_str()))
	//{
	//new_mat = new Material();
	//new_mat->path = lib_path;
	//new_mat->name = ass_tex_name;
	//
	//Texture* tex = nullptr;
	//tex = App->resources->material_importer->LoadTexture((*it).c_str(), false);
	//
	//if (tex)
	//new_mat->SetDiffuseTexture(tex);
	//
	//App->resources->material_importer->Import(new_mat, new_mat->name.c_str());
	//}
	//
	//string path_to_load = lib_path + string("\\") + lib_tex_name;
	//new_mat = App->resources->material_importer->LoadFromBinary(path_to_load.c_str());
	//new_mat->UnloadFromMemory();
	//}

	//std::vector<string> files = App->file_system->GetAllFilesInDirectory(App->file_system->GetTexturesPath().c_str(), true);
	//
	//for (auto it = files.begin(); it != files.end(); it++)
	//{
	//string lib_path = App->file_system->GetLibraryPath() + string("\\Materials");
	//
	//string ass_tex_name = App->file_system->GetLastPathItem((*it).c_str(), true);
	//string lib_tex_name = App->file_system->GetLastPathItem((*it).c_str(), false) + ".dds";
	//
	//Material* new_mat = nullptr;
	//
	//if (!App->file_system->IsFileInDirectory(lib_path.c_str(), lib_tex_name.c_str()))
	//{
	//new_mat = new Material();
	//new_mat->path = lib_path;
	//new_mat->name = ass_tex_name;
	//
	//Texture* tex = nullptr;
	//tex = App->resources->material_importer->LoadTexture((*it).c_str(), false);
	//
	//if (tex)
	//new_mat->SetDiffuseTexture(tex);
	//
	//App->resources->material_importer->Import(new_mat, new_mat->name.c_str());
	//}
	//
	//string path_to_load = lib_path + string("\\") + lib_tex_name;
	//new_mat = App->resources->material_importer->LoadFromBinary(path_to_load.c_str());
	//new_mat->UnloadFromMemory();
	//}
}

void MaterialImporter::ManageNewTexture(std::string path)
{
	string lib_path = App->file_system->GetLibraryPath() + string("\\Materials");
	string name = App->file_system->GetLastPathItem(path.c_str(), true); 
	
	Material* new_mat = nullptr;

	if (!App->file_system->IsFileInDirectory(lib_path.c_str(), name.c_str()))
	{
		new_mat = new Material();
		new_mat->path = lib_path;
		new_mat->name = name;

		Texture* tex = nullptr;
		tex = App->resources->material_importer->LoadTexture(path.c_str(), false);

		if (tex)
			new_mat->SetDiffuseTexture(tex);

		App->resources->material_importer->Import(new_mat, new_mat->name.c_str(), App->file_system->GetFileExtension(name.c_str()));

		Material* new_mat_res = (Material*)App->resources->CreateNewResource(RES_MATERIAL);
		new_mat_res = new_mat;

		App->resources->material_importer->FlipTexture(new_mat->diffuse);
	}

	string path_to_load = lib_path + string("\\Materials") + name;
	App->resources->material_importer->LoadFromBinary(path_to_load.c_str(), new_mat);
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

		list<Resource*> textures_resources = App->resources->GetResourcesByType(RES_MATERIAL);

		for (auto it = textures_resources.begin(); it != textures_resources.end(); it++, i++) 
		{
			string curr_name = (*it)->name + App->file_system->GetFileExtensionStr((*it)->path);

			if (ImGui::Selectable(curr_name.c_str()))
			{
				ComponentMaterial* mat = (ComponentMaterial*)App->scene->GetSelectedGameObject()->GetComponent(CMP_MATERIAL);

				if (mat != nullptr)
				{
					Material* new_mat = (Material*)(*it);
					if (new_mat->reference_counting == 0)
						new_mat->LoadToMemory(); 

					new_mat->reference_counting++; 
					mat->SetMaterial(new_mat);
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

bool MaterialImporter::Import(Material * mat_to_save, const char * tex_name, UID uid_to_save)
{
	bool ret = false; 

	//Get the path to save 
	file_extension ext = App->file_system->GetFileExtension(tex_name); 
	string tex_name_alone = App->file_system->GetLastPathItem(tex_name, false);

	string path_dst = App->file_system->GetAssetsPath() + '\\' + "Textures\\" + tex_name; 
	string path_to_save = App->file_system->GetLibraryPath() + '\\' + "Materials\\" + to_string(uid_to_save) + ".dds";

	if (ext == FX_DDS && mat_to_save != nullptr) //if the texture is already in dds we don't need to get the data and save it in dds format, we just copy the file
	{
		std::ifstream in; 
		in.open(path_dst.c_str(), ifstream::binary);

		std::ofstream out;            
		out.open(path_to_save.c_str(), ifstream::binary);
					
		// Get length of file:
		in.seekg(0, in.end);
		int length = in.tellg();
		in.seekg(0, in.beg);

		char* buf = new char[length];

		do {
			in.read(buf, length);
			out.write(buf, in.gcount()); 
		} while (in.gcount() > 0);         
											
		in.close();
		out.close();

		delete[] buf; 

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

void MaterialImporter::LoadFromBinary(const char * assets_tex_path, Material* mat_to_fill)
{	
	string meta_name = App->file_system->GetLastPathItem(assets_tex_path, true) + ".meta";
	string meta_file_path = App->file_system->GetTexturesPath() + "\\" + meta_name;

	if (!App->file_system->IsFileInDirectory(App->file_system->DeleteLastPathItem(assets_tex_path).c_str(), meta_name.c_str()))
	{
		CONSOLE_ERROR("Meta file not found in %s. Resource can't be load", assets_tex_path); 
		return; 
	}
	else
	{
		//Get the ID from the meta which is the name of the file 
		std::ifstream stream;
		stream.open(meta_file_path.c_str(), std::fstream::in);

		JSON_Value* root = json_parse_file(meta_file_path.c_str());
		JSON_Object* root_obj = json_value_get_object(root);

		UID obj_id = json_object_dotget_number(root_obj, "MetaInfo.UID");

		//Generate the path of the binary 
		string binary_path = App->file_system->GetLibraryPath() + "\\Materials\\" + to_string(obj_id) + ".dds"; 

		if (mat_to_fill)
		{
			Texture* new_tex = LoadTexture(binary_path.c_str(), false);
			mat_to_fill->SetDiffuseTexture(new_tex);
		}
	}
}

MaterialImporter::~MaterialImporter()
{
}

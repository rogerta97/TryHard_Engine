#include "TextureImporter.h"
#include "Application.h"

#include <iostream>
#include <filesystem>

#include "ComponentMaterial.h"


TextureImporter::TextureImporter()
{

}

bool TextureImporter::Start()
{
	ilInit(); 
	iluInit(); 
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	string go_symbol_path = string(App->file_system->GetTexturesPath() + "GameObjectIcon.png"); 
	LoadTexture(go_symbol_path.c_str(), true);

	return true;
}

bool TextureImporter::Update()
{
	return true;
}

bool TextureImporter::CleanUp()
{
	return true;
}

Texture* TextureImporter::LoadTexture(const char * path, bool not_flip)
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

	std::list<Texture*>::iterator tex_iterator;

	for (tex_iterator = textures_list.begin(); tex_iterator != textures_list.end(); tex_iterator++) {
		if (new_name.compare((*tex_iterator)->GetName()) == 0) {
			tex = (*tex_iterator);
		}
	}

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

			//Set the name 
			tex->SetName(App->file_system->GetLastPathItem(path).c_str()); 
			
			tex->Bind();
			tex->SetTextureSettings();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->GetWidth(), tex->GetHeight(), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			tex->UnBind();

			textures_list.push_back(tex); 
		}		
	}

	return tex; 
}

bool TextureImporter::SaveTexturesAsDDS()
{
	//for (auto it = textures_list.begin(); it != textures_list.end(); it++)
	//{
	//	SaveTexture((*it), IL_DDS); 
	//}

	return true;
}

bool TextureImporter::SaveTexture(Texture * tex_to_save, ILenum format_type)
{
	//Textures will be saved into DDS for the moment
	string textures_dir = App->file_system->GetLibraryPath() + "Textures\\";

	//First, we should check if the texture has been already saved in DDS format. (TODO: resource manager) For now we will always save a new one. 
	if (tex_to_save != nullptr)
	{
		switch (format_type)
		{
			case IL_DDS: 
			{
				//Save The texture
				{
					for (auto it = textures_list.begin(); it != textures_list.end(); it++)
					{
						//If the texture already exist in library, skip the saving process. 
						if (App->file_system->IsFileInDirectory(textures_dir.c_str(), (*it)->GetName().c_str()))
							continue;

						//If not, save it
						int size = ilSaveL(IL_DDS, NULL, 0);
						if (size > 0)
						{
							//Copy data
							GLubyte* data = new GLubyte[size];

							(*it)->Bind();
							data = ilGetData(); 

							if(ilSaveL(IL_DDS, data, size))
							{
								//Create the file 
								FILE* new_file;
								string save_dir = textures_dir + (*it)->GetName() + ".dds";
								new_file = fopen(save_dir.c_str(), "w");

								//Save the info 
								if (new_file != nullptr)
								{
									int result = fwrite(data, sizeof(ILubyte), sizeof(data), new_file);
								}

								fclose(new_file);
							}

							
						}
					}
				}		
				break; 
			}
		}
	}


	return true;
}

Texture* TextureImporter::DrawTextureList()
{
	static bool show_browser = false; 

	if (ImGui::Begin("Texture Browser", &show_browser))
	{		
		int i = 0; 

		std::vector<string> textures_on_folder = App->file_system->GetFilesInDirectory(App->file_system->GetTexturesPath().c_str()); 

		for (auto it = textures_on_folder.begin(); it != textures_on_folder.end(); it++, i++)
		{
			if ((*it) != "." && (*it) != "..")
			{
				ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen;
				ImGui::TreeNodeEx((*it).c_str(), node_flags);

				if(ImGui::IsItemClicked())
				{
					ComponentMaterial* mat = (ComponentMaterial*)App->scene->GetSelectedGameObject()->GetComponent(CMP_MATERIAL);

					if (mat != nullptr)
					{
						string file_path = App->file_system->GetTexturesPath() + (*it);
						mat->diffuse = LoadTexture(file_path.c_str());
					}

				}
			}
			

		}
		ImGui::End(); 
	}

	return nullptr; 
}


Texture * TextureImporter::GetTexture(const char* name)
{
	for (auto it = textures_list.begin(); it != textures_list.end(); it++)
	{
		if ((*it)->GetName() == string(name))
			return (*it); 
	}

	return nullptr; 
}

TextureImporter::~TextureImporter()
{
}

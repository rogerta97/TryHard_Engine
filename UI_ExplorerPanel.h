#pragma once
#include "UI_Panel.h"
#include "Texture.h"
#include <string>

class Material; 

class UI_ExplorerPanel : public UI_Panel
{
public:
	UI_ExplorerPanel();
	~UI_ExplorerPanel();

	void DrawExplorerRecursive(std::string folder, bool get_meta);

	bool Start();
	bool Update();
	bool CleanUp();

private:

	bool show_item_options; 
	std::string item_selected; 
	std::string root_folder; 
	
	Material* folder_mat; 
	Material* obj_mat; 
	Material* image_mat;
	Material* mesh_mat;
	Material* font_mat;
	Material* DDS_mat;
	Material* TGA_mat;
	Material* scene_mat;
};


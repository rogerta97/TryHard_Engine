#pragma once
#include "UI_Panel.h"
#include "Texture.h"
#include <string>

class UI_ExplorerPanel : public UI_Panel
{
public:
	UI_ExplorerPanel();
	~UI_ExplorerPanel();

	void DrawExplorerRecursive(std::string folder);

	bool Start();
	bool Update();
	bool CleanUp();

private:

	bool show_item_options; 
	std::string item_selected; 
	std::string root_folder; 
	
	Texture* folder_texture; 
	Texture* image_texture; 
	Texture* mesh_texture; 
	Texture* font_texture; 

};


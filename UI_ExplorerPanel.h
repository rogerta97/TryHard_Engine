#pragma once
#include "UI_Panel.h"
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

	std::string root_folder; 
};


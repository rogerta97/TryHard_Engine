#pragma once
#include "UI_Panel.h"
class UI_GamePanel :
	public UI_Panel
{
public:
	UI_GamePanel();
	~UI_GamePanel();

	bool Start();
	bool Update();
	bool CleanUp();

	ImVec2 pos;
	ImVec2 region_size;
};


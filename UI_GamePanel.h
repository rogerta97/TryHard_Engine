#pragma once
#include "UI_Panel.h"
enum ar_type { STANDARD, SQUARED, FREE_AR };

class UI_GamePanel :
	public UI_Panel
{

public:
	UI_GamePanel();
	~UI_GamePanel();

	bool Start();
	bool Update();
	bool CleanUp();

	ImVec2 CalculateSizeAndSetCursor(float original_aspect_ratio);
	void ShowARSelector();

public:
	ImVec2 pos;
	ImVec2 region_size;

	uint game_tex_width;
	uint game_tex_height;

	ar_type ar;


	float game_ar;
};


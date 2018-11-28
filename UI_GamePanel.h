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

private:
	ImVec2 CalculateSizeAndSetCursor(float original_aspect_ratio);

public:
	ImVec2 pos;
	ImVec2 region_size;
};


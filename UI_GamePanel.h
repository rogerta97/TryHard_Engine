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
	ImVec2 GetGameTexSize() const; //Uses imvec to avoid including glmath

public:
	ImVec2 pos;
	ImVec2 region_size;

	ImVec2 game_size;

	ar_type ar;

	bool size_changed;

	float game_ar;
};


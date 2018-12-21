#ifndef _UI_GAME_PANEL_H_
#define _UI_GAME_PANEL_H_

#include "UI_Panel.h"
#include "MathGeoLib\MathGeoLib.h"
enum ar_type { STANDARD, SQUARED, FREE_AR };
enum margin {NONE, VERTICAL, HORIZONTAL};

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

	ImVec2 GetMousePosInDock();
	ImVec2 GetMousePosInDockNormalized();
	float2 GetMousePosInDockZeroOne();

public:
	ImVec2 pos;
	ImVec2 region_size;

	ImVec2 game_size;

	ImVec2 game_tex_pos;

	margin margin_type;

	ar_type ar;

	bool size_changed;
	bool size_changed_last_frame = false; 

	float game_ar;

	bool is_mouse_in;
};

#endif


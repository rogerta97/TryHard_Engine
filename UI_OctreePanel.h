#pragma once
#include "UI_Panel.h"
class UI_OctreePanel : public UI_Panel
{
public:
	UI_OctreePanel();
	~UI_OctreePanel();

	bool Start();
	bool Update();
	bool CleanUp();
};


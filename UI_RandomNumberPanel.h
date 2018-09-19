#pragma once
#include "UI_Panel.h"

class UI_RandomNumberPanel : public UI_Panel
{
public:

	UI_RandomNumberPanel();
	~UI_RandomNumberPanel();

	bool Start();
	bool Update();

private: 

	float generated_num; 
};


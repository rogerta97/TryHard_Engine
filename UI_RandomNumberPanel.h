#pragma once
#include "UI_Panel.h"

enum RandomNumberType {RND_TYPE_INT, RND_TYPE_FLOAT};

class UI_RandomNumberPanel : public UI_Panel
{
public:

	UI_RandomNumberPanel();
	~UI_RandomNumberPanel();

	bool Start();
	bool Update();

private: 

	int max = 0, min = 0;
	float maxf = 0.0f, minf = 0.0f;
	bool show_warning; 

	float generated_num; 
	RandomNumberType random_type; 
};


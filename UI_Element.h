#pragma once

#include "GameObject.h"

class UI_Canvas;

enum UI_Widgget_Type
{
	UI_BUTTON,
	UI_CHECKBOX,
	UI_LABEL,
	UI_IMAGE,
	UI_INPUTFIELD,
	UI_NULL,
};

class UI_Element
{
public:
	UI_Element();
	~UI_Element();

	void Start(); 
	void Update();
	void CleanUp();

	UI_Widgget_Type GetType() const;
	void SetType(UI_Widgget_Type new_type);

private:

	UI_Widgget_Type wid_type; 
	UI_Canvas* canvas; 
};


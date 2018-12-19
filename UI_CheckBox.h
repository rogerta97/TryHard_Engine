#ifndef _UI_CHECKBOX_H_
#define _UI_CHECKBOX_H_

#include "UI_Element.h"

#include "ComponentCheckbox.h"

class UI_CheckBox : public UI_Element
{
public:
	UI_CheckBox(ComponentCheckBox* parent_cmp);
	~UI_CheckBox();

	ComponentCheckBox* cmp_container; 

	void Start();
	void Update();
	void CleanUp();
	void Draw();
};

#endif


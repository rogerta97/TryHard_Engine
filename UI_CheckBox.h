#ifndef _UI_CHECKBOX_H_
#define _UI_CHECKBOX_H_

#include "UI_Element.h"
#include <functional>
#include "ComponentCheckbox.h"

enum CheckBoxType
{ 
	CHT_TOGGLE_BOOL, 
	CHT_CUSTOM,
}; 

class UICallbackSystem; 

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

	bool GetIsOn() const;
	void SetIsOn(const bool& is_on); 

	CheckBoxType GetType() const;
	void SetType(const CheckBoxType& is_on);

private:

	bool is_on = true; 
	CheckBoxType checkbox_type = CHT_TOGGLE_BOOL; 
};

#endif


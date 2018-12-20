#ifndef _UI_CHECKBOX_H_
#define _UI_CHECKBOX_H_

#include "UI_Element.h"
#include <functional>
#include "ComponentCheckbox.h"

class UI_Button; 
class UI_Image; 

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
	bool Toggle();						//Toggle is_on boolean

	void SetImageVisible(const bool& value);

	CheckBoxType GetType() const;
	void SetType(const CheckBoxType& is_on);

	UI_Button* GetChildButton() const;
	void SetChildButton(UI_Button* is_on);

	UI_Image* GetToggleImage() const;
	void SetToggleImage(UI_Image* is_on);

private:

	UI_Button * child_button = nullptr; 
	UI_Image * img_to_toggle = nullptr;

	bool is_on = true; 
	CheckBoxType checkbox_type = CHT_TOGGLE_BOOL; 
};

#endif


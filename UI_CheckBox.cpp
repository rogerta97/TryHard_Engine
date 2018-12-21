#include "UI_CheckBox.h"
#include "UI_Image.h"
#include "ComponentImage.h"


UI_CheckBox::UI_CheckBox(ComponentCheckBox* parent_cmp)
{
	cmp_container = parent_cmp; 
	SetPercentage(0.3f); 
	checkbox_type = CHT_TOGGLE_BOOL; 

	child_button = nullptr;
	img_to_toggle = nullptr;
}


UI_CheckBox::~UI_CheckBox()
{
}

void UI_CheckBox::Start()
{
}

void UI_CheckBox::Update()
{
}

void UI_CheckBox::CleanUp()
{
}

void UI_CheckBox::Draw()
{
}

bool UI_CheckBox::GetIsOn() const
{
	return is_on;
}

void UI_CheckBox::SetIsOn(const bool & is_on)
{
	this->is_on = is_on; 
}

bool UI_CheckBox::Toggle()
{
	is_on = !is_on;
	SetImageVisible(is_on); 
	return is_on; 
}

void UI_CheckBox::SetImageVisible(const bool& value)
{
	ComponentImage* cm_img = (ComponentImage*)img_to_toggle->GetComponent(CMP_IMAGE); 
	cm_img->GetImage()->SetRenderElement(value);
}

CheckBoxType UI_CheckBox::GetType() const
{
	return checkbox_type;
}

void UI_CheckBox::SetType(const CheckBoxType & is_on)
{
	checkbox_type = is_on; 
}

GameObject * UI_CheckBox::GetChildButton() const
{
	return child_button;
}

void UI_CheckBox::SetChildButton(GameObject * is_on)
{
	child_button = is_on; 
}

GameObject * UI_CheckBox::GetToggleImage() const
{
	return img_to_toggle;
}

void UI_CheckBox::SetToggleImage(GameObject * img)
{
	img_to_toggle = img;
}

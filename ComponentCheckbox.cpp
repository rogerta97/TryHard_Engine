#include "ComponentCheckBox.h"
#include "UI_CheckBox.h"
#include "UI_Button.h"
#include "UI_Canvas.h"
#include "Application.h"
#include "GameObject.h"
#include "UICallbackSystem.h"

ComponentCheckBox::ComponentCheckBox(GameObject* parent)
{
	gameobject = parent; 
	component_type = CMP_CHECKBOX; 
	checkbox = new UI_CheckBox(this); 

	callback_system = new UICallbackSystem(this); 

	background_img_pos_percentage = { 0.15f, 0.50f };
	label_origin_pos_percentage = { 0.35f , 0.50f};

	toggle_done = false; 
}

ComponentCheckBox::~ComponentCheckBox()
{
}

bool ComponentCheckBox::Start()
{
	return false;
}

bool ComponentCheckBox::Update()
{
	if (App->GetGameState() != GameState::RUNNING)
		return false; 

	//Check if the child button is pressed, in that case, we switch is_on
	if (GetCheckBox()->GetChildButton() != nullptr)
	{
		if (GetCheckBox()->GetChildButton()->GetState() == UI_ElementState::ELM_PRESSED)
		{
			if (!toggle_done)
			{
				GetCheckBox()->Toggle();
				toggle_done = true; 
			}	

			DoActions();
		}

		if (GetCheckBox()->GetChildButton()->GetState() == UI_ElementState::ELM_UP)
		{		
			toggle_done = false;
		}
			
	}


	return false;
}

bool ComponentCheckBox::CleanUp()
{
	UI_Canvas* canvas_container = checkbox->GetCanvas();
	canvas_container->DeleteElementByUID(gameobject->unique_id);

	return false;
}

void ComponentCheckBox::Draw(bool is_editor)
{

}

void ComponentCheckBox::FitToRect()
{
}

void ComponentCheckBox::OnEvent(const Event & new_event)
{
	switch (new_event.type)
	{
	case EventType::PLAY:
		BindCallbackFunctions(); 
		break; 

	}
}

void ComponentCheckBox::Load(JSON_Object * json_obj)
{
}

void ComponentCheckBox::Save(JSON_Object * json_obj, const char * root)
{
}

float2 ComponentCheckBox::GetBackgroundDistancePercentage()
{
	return background_img_pos_percentage;
}

float2 ComponentCheckBox::GetLabelPercentage()
{
	return label_origin_pos_percentage;
}

UI_CheckBox * ComponentCheckBox::GetCheckBox() const
{
	return checkbox;
}

void ComponentCheckBox::BindCallbackFunctions()
{
	int counter = 0;
	for (auto it = callback_system->GetCallbacks().begin(); it != callback_system->GetCallbacks().end(); it++)
	{
		if ((*it)->action_bool != nullptr)
		{
			bool init_toggle_value = (*it)->value_bool; 

			std::function<void()> on_binded_func = std::bind((*it)->action_bool, (*it)->value_bool);
			std::function<void()> off_binded_func = std::bind((*it)->action_bool, !(*it)->value_bool);

			(*it)->action = on_binded_func;

			ButtonOnAction[counter] = on_binded_func;
			ButtonOffAction.push_back(off_binded_func);
			++counter; 
		}
	}
}

void ComponentCheckBox::DoActions()
{
	if (GetCheckBox()->GetIsOn() && !ButtonOnAction.empty())
	{
		for (auto it = ButtonOnAction.begin(); it != ButtonOnAction.end(); it++)
		{
			std::function<void()> curr_func = (*it);
			curr_func();
		}
	}
	else if(!ButtonOffAction.empty())
	{		
		for (auto it = ButtonOffAction.begin(); it != ButtonOffAction.end(); it++)
		{
			std::function<void()> curr_func = (*it);
			curr_func();
		}
	}
		
	

}

#include "ComponentCheckBox.h"
#include "UI_CheckBox.h"
#include "UI_Button.h"
#include "UI_Canvas.h"
#include "ComponentCanvas.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentButton.h"
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
		ComponentButton* button = (ComponentButton*)GetCheckBox()->GetChildButton()->GetComponent(CMP_BUTTON);

		if (button->GetButton()->GetState() == UI_ElementState::ELM_PRESSED)
		{
			if (!toggle_done)
			{
				GetCheckBox()->Toggle();
				toggle_done = true; 
			}	

			DoActions();
		}

		if (button->GetButton()->GetState() == UI_ElementState::ELM_UP)
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
	ComponentCanvas* cmp_canvas = nullptr;

	UID canvas_go_uid = json_object_dotget_number(json_obj, "CanvasContainer");
	GameObject* container_canvas_go = App->scene->GetGameObjectByID(canvas_go_uid);

	if (container_canvas_go)
	{
		cmp_canvas = (ComponentCanvas*)container_canvas_go->GetComponent(CMP_CANVAS);
		if (cmp_canvas)
		{
			GetCheckBox()->SetCanvas(cmp_canvas->GetCanvas());
			cmp_canvas->AddElement(gameobject);
		}
	}

	checkbox->SetIsOn(json_object_dotget_boolean(json_obj, "IsOn"));

	checkbox->child_button_uid = json_object_dotget_number(json_obj, "ButtonUID");
	checkbox->img_to_toggle_uid = json_object_dotget_number(json_obj, "ImageUID");

	int num_actions = json_object_dotget_number(json_obj, "NumActions");

	for (int i = 0; i < num_actions; i++)
	{
		string action_name = json_object_dotget_string(json_obj, string("Actions.ActionName_" + to_string(i)).c_str());

		UI_CallbackAgent* new_agent = new UI_CallbackAgent(callback_system);
		new_agent->SetEmpty();

		bool value = json_object_dotget_boolean(json_obj, string("Actions.ActionValue_" + to_string(i)).c_str());
		new_agent->value_bool = value;

		if (action_name == "SetVsync(bool)")
			new_agent->action_bool = [](bool newValue) { App->SetVsync(newValue); };

		new_agent->name = action_name; 

		//Create true & false version
		std::function<void()> binded_func = std::bind(new_agent->action_bool, new_agent->value_bool);
		ButtonOnAction.push_back(binded_func);

		binded_func = std::bind(new_agent->action_bool, !new_agent->value_bool);
		ButtonOffAction.push_back(binded_func);

		callback_system->AddAgent(new_agent);
	}
}

void ComponentCheckBox::Save(JSON_Object * json_obj, const char * root)
{
	std::string item_name = root + std::string(".ComponentCheckBox");

	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	int uid = GetCheckBox()->GetContainerCanvasGO()->unique_id;

	json_object_dotset_number(json_obj, std::string(item_name + ".CanvasContainer").c_str(), uid);

	json_object_dotset_boolean(json_obj, std::string(item_name + ".IsOn").c_str(), checkbox->GetIsOn());

	json_object_dotset_number(json_obj, std::string(item_name + ".ButtonUID").c_str(), checkbox->GetChildButton()->unique_id);
	json_object_dotset_number(json_obj, std::string(item_name + ".ImageUID").c_str(), checkbox->GetToggleImage()->unique_id);

	json_object_dotset_number(json_obj, std::string(item_name + ".NumActions").c_str(), callback_system->GetCallbacks().size());

	item_name += ".Actions";

	int counter = 0;
	for (auto it = callback_system->GetCallbacks().begin(); it != callback_system->GetCallbacks().end(); it++)
	{
		if ((*it)->parent != nullptr)
			json_object_dotset_number(json_obj, std::string(item_name + ".Target_" + to_string(counter)).c_str(), (*it)->parent->unique_id);
		else
			json_object_dotset_number(json_obj, std::string(item_name + ".Target_" + to_string(counter)).c_str(), 0);

		json_object_dotset_string(json_obj, std::string(item_name + ".ActionName_" + to_string(counter)).c_str(), (*it)->name.c_str());

		if ((*it)->action_bool != nullptr)
		{
			json_object_dotset_boolean(json_obj, std::string(item_name + ".ActionValue_" + to_string(counter)).c_str(), (*it)->value_bool);
		}
		else
		{
			json_object_dotset_string(json_obj, std::string(item_name + ".ActionName_" + to_string(counter)).c_str(), "Empty");
			json_object_dotset_string(json_obj, std::string(item_name + ".ActionArg_" + to_string(counter)).c_str(), "Empty");
		}

	}

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

#include "ComponentButton.h"
#include "ComponentRectTransform.h"
#include "ComponentCanvas.h"
#include "ComponentImage.h"
#include "UICallbackSystem.h"

#include "Application.h"

#include "UI_Button.h"
#include "UI_Plane.h"
#include "UI_Canvas.h"
#include "UI_Image.h"

ComponentButton::ComponentButton(GameObject * parent)
{
	gameobject = parent; 
	component_type = CMP_BUTTON; 
	button = new UI_Button(this); 
	callback_system = new UICallbackSystem(this); 

	has_mouse_entered = false; 

	hover_color = { 0.9f, 0.9f, 0.9f }; 
	pressed_color = { 0.7f, 0.7f, 0.7f };
}

ComponentButton::~ComponentButton()
{
}

bool ComponentButton::Start()
{
	return false;
}

bool ComponentButton::Update()
{
	button->Update(); 

	return true;
}

bool ComponentButton::CleanUp()
{
	// Delete object from the canvas 
	UI_Canvas* canvas_container = button->GetCanvas();
	canvas_container->DeleteElementByUID(gameobject->unique_id);

	// Callback System 
	OnMousePressed.clear();
	callback_system->CleanSystem();
	delete callback_system; 

	button->CleanUp();
	delete button; 

	return true;
}

void ComponentButton::Draw(bool is_editor)
{
	App->renderer3D->UseUIRenderSettings();

	if(button)
		button->Draw(is_editor);
}

void ComponentButton::FitToRect()
{
	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	button->GetArea()->Resize(rtransform->width, rtransform->height);
}

void ComponentButton::OnEvent(const Event & new_event)
{
	if (new_event.type == EventType::PLAY)
	{
		BindCallbackFunctions(); 
	}

	if (App->GetGameState() != RUNNING)
	{
		SetImageColor({ 1,1,1 });
		return;
	}
		
	if (new_event.button.but == GetButton())
	{
		switch (new_event.type)
		{
		case EventType::UI_ELEMENT_DOWN:
			GetButton()->SetState(ELM_PRESSED);
			SetImageColor(pressed_color);
			break;

		case EventType::UI_ELEMENT_UP:
			GetButton()->SetState(ELM_UP);
			SetImageColor(hover_color);
			break;

		case EventType::UI_ELEMENT_ENTER:
			GetButton()->SetState(ELM_HOVERED);
			SetImageColor(hover_color);
			break;

		case EventType::UI_ELEMENT_OUT:
			GetButton()->SetState(ELM_IDLE);
			SetImageColor({1,1,1});
			break;
		}
	}
}

void ComponentButton::Load(JSON_Object * json_obj)
{
	ComponentCanvas* cmp_canvas = nullptr;

	UID canvas_go_uid = json_object_dotget_number(json_obj, "CanvasContainer");
	GameObject* container_canvas_go = App->scene->GetGameObjectByID(canvas_go_uid);

	if (container_canvas_go)
	{
		cmp_canvas = (ComponentCanvas*)container_canvas_go->GetComponent(CMP_CANVAS);
		if (cmp_canvas)
		{
			GetButton()->SetCanvas(cmp_canvas->GetCanvas());
			cmp_canvas->AddElement(gameobject);
		}
	}

	float2 size = { (float)json_object_dotget_number(json_obj, "AreaSize.X"), (float)json_object_dotget_number(json_obj, "AreaSize.Y") };
	
	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	rtransform->Resize(size);
}

void ComponentButton::Save(JSON_Object * json_obj, const char * root)
{
	std::string item_name = root + std::string(".ComponentButton"); 

	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	UID uid = GetButton()->GetContainerCanvasGO()->unique_id;

	json_object_dotset_number(json_obj, std::string(item_name + ".CanvasContainer").c_str(), uid);

	json_object_dotset_number(json_obj, std::string(item_name + ".AreaSize.X").c_str(), rtransform->width);
	json_object_dotset_number(json_obj, std::string(item_name + ".AreaSize.Y").c_str(), rtransform->height);

	json_object_dotset_number(json_obj, std::string(item_name + ".NumActions").c_str(), callback_system->GetCallbacks().size());

	item_name += ".Actions"; 

	for (auto it = callback_system->GetCallbacks().begin(); it != callback_system->GetCallbacks().end(); it++)
	{
		if((*it)->parent != nullptr)
			json_object_dotset_number(json_obj, std::string(item_name + ".Target").c_str(), (*it)->parent->unique_id);
		else
			json_object_dotset_number(json_obj, std::string(item_name + ".Target").c_str(), 0);

		if((*it)->action != nullptr)
			json_object_dotset_string(json_obj, std::string(item_name + ".Action").c_str(), (*it)->name.c_str());
		else
			json_object_dotset_string(json_obj, std::string(item_name + ".Action").c_str(), "Empty");
	}
}

UI_Button * ComponentButton::GetButton() const
{
	return button;
}

void ComponentButton::BindCallbackFunctions()
{
	int counter = 0; 
	for (auto it = callback_system->GetCallbacks().begin(); it != callback_system->GetCallbacks().end(); it++, counter++)
	{
		if ((*it)->action_char != nullptr)
		{
			std::function<void()> binded_func = std::bind((*it)->action_char, (*it)->value_char);
			(*it)->action = binded_func; 
			OnMousePressed[counter] = (*it)->action;
		}
	}
}

float3 ComponentButton::GetHoverColor() const
{
	return hover_color;
}

void ComponentButton::SetHoverColor(const float3 & new_color)
{	
	hover_color = new_color; 	
}

float3 ComponentButton::GetPressedColor() const
{
	return pressed_color;
}

void ComponentButton::SetPressedColor(const float3 & new_color)
{
	pressed_color = new_color; 
}

void ComponentButton::SetImageColor(const float3 & new_color)
{
	ComponentImage* cmp_img = (ComponentImage*)gameobject->GetComponent(CMP_IMAGE);

	if (cmp_img)
		cmp_img->GetImage()->image_color = new_color;
}

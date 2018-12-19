#include "ComponentButton.h"
#include "ComponentRectTransform.h"
#include "ComponentCanvas.h"
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
	for (auto it = callback_system->GetCallbacks().begin(); it != callback_system->GetCallbacks().end(); it++)
	{
		if ((*it)->action_char != nullptr)
		{
			std::function<void()> binded_func = std::bind((*it)->action_char, (*it)->value_char);
			OnMousePressed.push_back(binded_func);
		}
	}
}

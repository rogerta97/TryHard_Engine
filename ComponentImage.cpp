#include "ComponentImage.h"
#include "ComponentRectTransform.h"
#include "ComponentCanvas.h"
#include "UI_Plane.h"
#include "UI_Image.h"
#include "UI_Canvas.h"

#include "Material.h"
#include "Application.h"

#include "GameObject.h"
#include "mmgr\mmgr.h"

ComponentImage::ComponentImage(GameObject* parent)
{
	gameobject = parent; 
	component_type = CMP_IMAGE; 
}

ComponentImage::~ComponentImage()
{
	image->CleanUp();
	delete image; 
}

bool ComponentImage::Start()
{
	image = new UI_Image(this); 
	return false;
}

bool ComponentImage::Update()
{
	if(image)
		image->Update(); 

	return false;
}

bool ComponentImage::CleanUp()
{
	// Delete object from the canvas 
	UI_Canvas* canvas_container = image->GetCanvas();
	canvas_container->DeleteElementByUID(gameobject->unique_id); 
	
	return false;
}

void ComponentImage::Draw(bool is_editor)
{
	App->renderer3D->UseUIRenderSettings();
	image->Draw(); 
}

void ComponentImage::OnEvent(const Event & new_event)
{
	switch (new_event.type)
	{
	case EventType::INTERPOLATE_ALPHA:
		GetImage()->SetAlphaPercentage(new_event.alpha_lvl.percentage);
		break;
	}
}

void ComponentImage::Load(JSON_Object * json_obj)
{
	ComponentCanvas* cmp_canvas = nullptr;

	UID canvas_go_uid = json_object_dotget_number(json_obj, "CanvasContainer"); 
	GameObject* container_canvas_go = App->scene->GetGameObjectByID(canvas_go_uid);
	
	if (container_canvas_go)
	{
		cmp_canvas = (ComponentCanvas*)container_canvas_go->GetComponent(CMP_CANVAS);
		if (cmp_canvas)
		{
			GetImage()->SetCanvas(cmp_canvas->GetCanvas());
			cmp_canvas->AddElement(gameobject);
		}
	}
					
	float2 size = { (float)json_object_dotget_number(json_obj, "PlaneSize.X"), (float)json_object_dotget_number(json_obj, "PlaneSize.Y") };

	std::string mat_name = json_object_dotget_string(json_obj, "MaterialName");

	Material* img_mat = (Material*)App->resources->Get(RES_MATERIAL, mat_name.c_str());

	if (img_mat->reference_counting == 0)
	{
		img_mat->LoadToMemory();
	}
	img_mat->reference_counting++; 

	GetImage()->SetMaterial(img_mat);

	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	rtransform->Resize(size); 
}

void ComponentImage::Save(JSON_Object * json_obj, const char * root)
{

	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM); 

	std::string item_name = root + std::string(".ComponentImage"); 
	
	//Plane will have the size of the rectransform
	json_object_dotset_number(json_obj, std::string(item_name + ".CanvasContainer").c_str(), GetImage()->GetContainerCanvasGO()->unique_id);
	json_object_dotset_number(json_obj, std::string(item_name + ".PlaneSize.X").c_str(), rtransform->width); 
	json_object_dotset_number(json_obj, std::string(item_name + ".PlaneSize.Y").c_str(), rtransform->height);
	json_object_dotset_string(json_obj, std::string(item_name + ".MaterialName").c_str(), GetImage()->GetMaterial()->name.c_str());
}

UI_Image * ComponentImage::GetImage() const
{
	return image;
}

void ComponentImage::FitToRect()
{
	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM); 
	image->GetPlane()->Resize(rtransform->width, rtransform->height); 	
}

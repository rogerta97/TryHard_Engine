#include "ComponentText.h"
#include "UI_Label.h"
#include "UI_TextInput.h"
#include "DebugDraw.h"
#include "Font.h"

#include "ComponentCanvas.h"
#include "ComponentTextInput.h"
#include "ComponentRectTransform.h"
#include "UI_Canvas.h"

#include "Application.h"

ComponentText::ComponentText(GameObject* parent)
{
	component_type = CMP_TEXT;
	gameobject = parent;

	label = new UI_Label(this); 
	label->SetText("SampleText"); 

	horizontal_overflow = HORIZONTAL_WRAP;
	vertical_overflow = VERTICAL_TRUNCATE; 

	line_spacing = GetContainerPlaneSize().y * 1.25f;
	label->section = { -1, -1 };
	draw_color = { 0,0,0 }; 
	default_size = { 120,50 };
}	


ComponentText::~ComponentText()
{
}

bool ComponentText::Start()
{
	return true;
}

bool ComponentText::Update()
{
	return false;
}

bool ComponentText::CleanUp()
{
	UI_Canvas* canvas_container = label->GetCanvas();
	canvas_container->DeleteElementByUID(gameobject->unique_id);

	delete label; 

	return false;
}

void ComponentText::Draw(bool is_editor)
{
	label->Draw(is_editor); 

	if (!is_editor)
		return; 

	RenderContainerPlane();

}

void ComponentText::OnEvent(const Event & new_event)
{
	switch (new_event.type)
	{
	case EventType::INTERPOLATE_ALPHA:
		GetLabel()->SetAlphaPercentage(new_event.alpha_lvl.percentage);
		break;
	}
}

void ComponentText::Load(JSON_Object * json_obj)
{
	ComponentCanvas* cmp_canvas = nullptr;

	UID canvas_go_uid = json_object_dotget_number(json_obj, "CanvasContainer");
	GameObject* container_canvas_go = App->scene->GetGameObjectByID(canvas_go_uid);

	if (container_canvas_go)
	{
		cmp_canvas = (ComponentCanvas*)container_canvas_go->GetComponent(CMP_CANVAS);
		if (cmp_canvas)
		{
			GetLabel()->SetCanvas(cmp_canvas->GetCanvas());
			cmp_canvas->AddElement(gameobject);
		}
	}

	text_origin.x = json_object_dotget_number(json_obj, "Origin.X");
	text_origin.y = json_object_dotget_number(json_obj, "Origin.Y");

	section.x = json_object_dotget_number(json_obj, "Section.X");
	section.y = json_object_dotget_number(json_obj, "Section.Y");

	draw_color.x = json_object_dotget_number(json_obj, "Color.R");
	draw_color.y = json_object_dotget_number(json_obj, "Color.G");
	draw_color.z = json_object_dotget_number(json_obj, "Color.B");

	GetLabel()->color = draw_color; 

	int type = json_object_dotget_number(json_obj, "HorizontalOverflow");
	horizontal_overflow = (horizontalTextOverflow)type; 

	type = json_object_dotget_number(json_obj, "VerticalOverflow");
	vertical_overflow = (verticalTextOverflow)type;

	type = json_object_dotget_number(json_obj, "Clipping");
	clipping = (ClipTextType)type;

	UID input_parent_uid = json_object_dotget_number(json_obj, "ParentInputUID");

	if (App->scene->GetGameObjectByID(input_parent_uid))
	{
		ComponentTextInput* cmp_ti = (ComponentTextInput*)App->scene->GetGameObjectByID(input_parent_uid)->GetComponent(CMP_TEXTINPUT);

		if (cmp_ti->GetPlaceHolderUID() == GetGameObject()->unique_id)
			cmp_ti->GetInputField()->SetPlaceHolderText(GetGameObject());

		if (cmp_ti->GetShowTextUID() == GetGameObject()->unique_id)
			cmp_ti->GetInputField()->SetShowText(GetGameObject());
	}
		

	std::string font_name = json_object_dotget_string(json_obj, "FontName");
	label->text_size = json_object_dotget_number(json_obj, "Size");

	label->SetText(json_object_dotget_string(json_obj, "Text"));
	SetClipping(clipping);
}

void ComponentText::Save(JSON_Object * json_obj, const char * root)
{
	std::string item_name = root + std::string(".ComponentText");

	json_object_dotset_number(json_obj, std::string(item_name + ".Origin.X").c_str(), label->GetOrigin().x);
	json_object_dotset_number(json_obj, std::string(item_name + ".Origin.Y").c_str(), label->GetOrigin().y);

	json_object_dotset_number(json_obj, std::string(item_name + ".Section.X").c_str(), section.x);
	json_object_dotset_number(json_obj, std::string(item_name + ".Section.Y").c_str(), section.y);

	json_object_dotset_number(json_obj, std::string(item_name + ".Color.R").c_str(), GetLabel()->color.x);
	json_object_dotset_number(json_obj, std::string(item_name + ".Color.G").c_str(), GetLabel()->color.y);
	json_object_dotset_number(json_obj, std::string(item_name + ".Color.B").c_str(), GetLabel()->color.z);

	json_object_dotset_number(json_obj, std::string(item_name + ".HorizontalOverflow").c_str(), horizontal_overflow);
	json_object_dotset_number(json_obj, std::string(item_name + ".VerticalOverflow").c_str(), vertical_overflow);

	json_object_dotset_number(json_obj, std::string(item_name + ".Clipping").c_str(), clipping);

	json_object_dotset_number(json_obj, std::string(item_name + ".ParentInputUID").c_str(), GetParentTextInputUID());

	json_object_dotset_string(json_obj, std::string(item_name + ".Text").c_str(), label->GetText().c_str());

	json_object_dotset_string(json_obj, std::string(item_name + ".FontName").c_str(), label->GetFont().name.c_str());
	json_object_dotset_number(json_obj, std::string(item_name + ".Size").c_str(), label->GetFont().size);

	json_object_dotset_number(json_obj, std::string(item_name + ".CanvasContainer").c_str(), label->GetContainerCanvasGO()->unique_id);

}

void ComponentText::RenderContainerPlane()
{
	App->renderer3D->UseDebugRenderSettings();

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(container_plane_vertices[0].x, container_plane_vertices[0].y, container_plane_vertices[0].z);
	glVertex3f(container_plane_vertices[1].x, container_plane_vertices[1].y, container_plane_vertices[1].z);

	glVertex3f(container_plane_vertices[0].x, container_plane_vertices[0].y, container_plane_vertices[0].z);
	glVertex3f(container_plane_vertices[2].x, container_plane_vertices[2].y, container_plane_vertices[2].z);

	glVertex3f(container_plane_vertices[2].x, container_plane_vertices[2].y, container_plane_vertices[2].z);
	glVertex3f(container_plane_vertices[3].x, container_plane_vertices[3].y, container_plane_vertices[3].z);

	glVertex3f(container_plane_vertices[3].x, container_plane_vertices[3].y, container_plane_vertices[3].z);
	glVertex3f(container_plane_vertices[1].x, container_plane_vertices[1].y, container_plane_vertices[1].z);

	glEnd();
}

float3 ComponentText::GetContainerPlaneCenter()
{
	float3 total = container_plane_vertices[0] + container_plane_vertices[1] + container_plane_vertices[2] + container_plane_vertices[3];
	return total / 4; 
}

float2 ComponentText::GetContainerPlaneSize()
{
	float2 ret; 

	ret.x = (container_plane_vertices[1] - container_plane_vertices[0]).Length(); 
	ret.y = (container_plane_vertices[2] - container_plane_vertices[0]).Length();

	return ret;
}

float3 ComponentText::GetClippingDistance(const ClipTextType new_clip)
{
	// 1. Get the distance that the planes should move deppending on clipping type
	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	float3 translation = { 0,0,0 };

	float3 p1, p2;

	switch (clipping)
	{
	case ClipTextType::CLIP_TOPLEFT:
	{
		p1 = rtransform->GetGlobalPosition() + float3({ -rtransform->width / 2, rtransform->height / 2, 0 });
		p2 = container_plane_vertices[0];
		translation = p1 - p2;
	}

	break;

	case ClipTextType::CLIP_CENTER:
	{
		p1 = rtransform->GetGlobalPosition();
		p2 = GetContainerPlaneCenter();
		translation = p1 - p2;
	}

	break;

	case ClipTextType::CLIP_MIDDLELEFT:
	{
		p1 = rtransform->GetGlobalPosition() - float3(rtransform->width / 2.0f, 0,0);
		float container_width = float3(container_plane_vertices[1] - container_plane_vertices[0]).Length(); 
		p2 = GetContainerPlaneCenter() - float3({container_width / 2.0f, 0, 0});
		translation = p1 - p2;
	}

	break;
	}

	return translation; 
}




float3 ComponentText::GetCursorPosFromLetter(const int& pos)
{
	// If the text is empty the position is the text origin
	float3 return_pos = { 0,0,0 };
	std::vector<float3> offset_list = label->GetOffsetList();

	if (label->GetText() != "")
	{
		int counter = 0;
		bool entered = false;
		for (auto it = offset_list.begin(); it != offset_list.end(); it++)
		{
			return_pos += (*it);

			if (counter++ == pos) //We reached the letter we want to lay
			{
				std::string text = label->GetText();
				Character* curr_char = label->GetFont().GetCharacter(text[pos]);
				return_pos.x -= curr_char->Advance / 2.0f;
				entered = true;
				break;
			}
		}

		if (!entered) //The cursor is in the last one
		{
			Character* curr_char = label->GetFont().GetCharacter(label->GetText()[counter]);
			return_pos.x += curr_char->Advance / 2.0f;
		}

	}

	return_pos += float3(GetLabel()->GetOrigin().x, GetLabel()->GetOrigin().y, 0);
	return_pos.y = 0;

	//We add the text transform position to send the point in global coords 
	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	return_pos += rtransform->GetGlobalPosition();	
	
	return return_pos;
}

UI_Label * ComponentText::GetLabel() const
{
	return label;
}

void ComponentText::UpdateContainerPlane()
{

}

float2 ComponentText::GetSection() const
{
	return label->section;
}

void ComponentText::SetSection(const float2 new_section)
{
	label->section = new_section; 
}

void ComponentText::MoveSectionForward()
{
	section += {1, 1}; 
}

void ComponentText::MovesectionBackward()
{
	section += {-1, -1};
}

horizontalTextOverflow ComponentText::GetHorizontalOverflow() const
{
	return horizontal_overflow;
}

void ComponentText::SetHorizontalOverflow(const horizontalTextOverflow new_ov)
{
	horizontal_overflow = new_ov;
}

verticalTextOverflow ComponentText::GetVerticalOverflow() const
{
	return vertical_overflow;
}

void ComponentText::SetVerticalOverflow(const verticalTextOverflow new_ov)
{
	vertical_overflow = new_ov;
}

UID ComponentText::GetParentTextInputUID() const
{
	return text_input_ID;
}

void ComponentText::SetParentTextInputUID(const UID & new_ov)
{
	text_input_ID = new_ov; 
}

ClipTextType ComponentText::GetClipping() const
{
	return clipping;
}

void ComponentText::SetClipping(const ClipTextType new_clip)
{
	clipping = new_clip; 
	float3 translation = GetClippingDistance(clipping);

	// 2. Move every text plane 
	if (label && translation.x != 0 || translation.y != 0)
			TranslateEnclosedPlane(translation);			
			
}

void ComponentText::TranslateEnclosedPlane(float3 increment)
{
	container_plane_vertices[0] += increment;
	container_plane_vertices[1] += increment;
	container_plane_vertices[2] += increment;
	container_plane_vertices[3] += increment;

	label->TranslateOrigin(float2(increment.x, increment.y)); 
}



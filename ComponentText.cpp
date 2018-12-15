#include "ComponentText.h"
#include "UI_Label.h"
#include "DebugDraw.h"
#include "Font.h"

#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "UI_Canvas.h"

#include "Application.h"

ComponentText::ComponentText(GameObject* parent)
{
	component_type = CMP_TEXT;
	gameobject = parent;
	label = new UI_Label(this); 
	label->SetText("Insert Text"); 
	line_spacing = label->GetFont().size * 1.3f; 
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
	return false;
}

void ComponentText::Draw(bool is_editor)
{
	label->Draw(is_editor); 

	if (!is_editor)
		return; 

	RenderContainerPlane();

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


UI_Label * ComponentText::GetLabel() const
{
	return label;
}

void ComponentText::UpdateContainerPlane()
{

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

ClipTextType ComponentText::GetClipping() const
{
	return clipping;
}

void ComponentText::SetClipping(const ClipTextType new_clip)
{
	clipping = new_clip; 

	// 1. Get the distance that the planes should move deppending on clipping type
	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	float3 translation = {0,0,0};

	float3 p1, p2; 

	switch (clipping)
	{
		case ClipTextType::CLIP_TOPLEFT:
		{
			p1 = rtransform->GetGlobalPosition() + float3({-rtransform->width / 2, rtransform->height / 2, 0});
			p2 = container_plane_vertices[0];
			translation = p1 - p2;
		}

		break;

		case ClipTextType::CLIP_BOTTOMLEFT:
		{
			p1 = rtransform->GetGlobalPosition() + float3({ -rtransform->width / 2, -rtransform->height / 2, 0 });
			p2 = container_plane_vertices[2];
			translation = p1 - p2;
		}

		break;

		case ClipTextType::CLIP_TOPRIGHT:
		{
			p1 = rtransform->GetGlobalPosition() + float3({ rtransform->width / 2, rtransform->height / 2, 0 });
			p2 = container_plane_vertices[1];
			translation = p1 - p2;
		}

		break;

		case ClipTextType::CLIP_BOTTOMRIGHT:
		{
			p1 = rtransform->GetGlobalPosition() + float3({ rtransform->width / 2, -rtransform->height / 2, 0 });
			p2 = container_plane_vertices[3];
			translation = p1 - p2;
		}

		case ClipTextType::CLIP_CENTER:
		{
			p1 = rtransform->GetGlobalPosition();
			p2 = GetContainerPlaneCenter();
			translation = p1 - p2;
		}

		break;
	}

	// 2. Move every text plane 
	if (translation.x != 0 || translation.y != 0)
	{
		if(label)
			label->TranslateCharactersPlanes(translation); //Move all planes in that increment
	}		
		
	
}

void ComponentText::TranslateEnclosedPlane(float3 increment)
{
	container_plane_vertices[0] += increment;
	container_plane_vertices[1] += increment;
	container_plane_vertices[2] += increment;
	container_plane_vertices[3] += increment;
}								

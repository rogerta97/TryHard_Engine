#include "ComponentText.h"
#include "UI_Label.h"
#include "DebugDraw.h"

#include "ComponentCanvas.h"
#include "ComponentRectTransform.h"
#include "UI_Canvas.h"

#include "Application.h"

ComponentText::ComponentText(GameObject* parent)
{
	component_type = CMP_TEXT;
	gameobject = parent;
	label = new UI_Label(this); 
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


UI_Label * ComponentText::GetLabel() const
{
	return label;
}

ClipTextType ComponentText::GetClipping() const
{
	return clipping;
}

void ComponentText::SetClipping(const ClipTextType new_clip)
{
	clipping = new_clip; 

	// Get the increment in which we have to translate the text planes
	// 1. Create the container plane -----
	label->CreateEnclosedPlane(container_plane_vertices);

	// --------
	// 2. Get the distance that the planes should move deppending on clipping type
	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM);
	float3 translation = {0,0,0};

	switch (clipping)
	{
		case ClipTextType::CLIP_TOPLEFT:
		{
			float3 top_left_rtransform = rtransform->GetGlobalPosition() + float3({-rtransform->width / 2, rtransform->height / 2, 0});
			float3 top_left_rect = container_plane_vertices[0]; 
			translation = top_left_rtransform - top_left_rect;
		}
		break;
	}

	// 3. Move every text plane 
	if (translation.x != 0 || translation.y != 0) //Move all planes in that increment		
		label->TranslateCharactersPlanes(translation); 
	
}

void ComponentText::TranslateEnclosedPlane(float3 increment)
{
	container_plane_vertices[0] += increment; 
	container_plane_vertices[1] += increment;
	container_plane_vertices[2] += increment;
	container_plane_vertices[3] += increment;
}

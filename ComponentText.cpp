#include "ComponentText.h"
#include "UI_Label.h"
#include "DebugDraw.h"

#include "Application.h"

ComponentText::ComponentText(GameObject* parent)
{
	component_type = CMP_TEXT;
	gameobject = parent;
	label = new UI_Label(this); 
	SetClipping(CLIP_TOPLEFT);
}


ComponentText::~ComponentText()
{
}

bool ComponentText::Start()
{
	return false;
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

	switch (clipping)
	{
		case ClipTextType::CLIP_TOPLEFT:
		{
			float3 xmx = label->GetValueFromRenderedText("YMax"); 

		}
		break;

	}
}

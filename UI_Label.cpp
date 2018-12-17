#include "UI_Label.h"
#include "UI_Image.h"
#include "UI_Plane.h"

#include "ComponentText.h"
#include "ComponentTransform.h"
#include "ComponentRectTransform.h"

#include "MathGeoLib\MathGeoLib.h"

#include "Application.h"

#define VERTICAL_LIMIT_OFFSET 10 //When truncating vertical text, truncate when with is "a little bit more" than the origin

UI_Label::UI_Label(ComponentText* cmp_text)
{
	cmp_container = cmp_text; 
	SetFont("Antonio-Regular");
	text_size = font.size;
	SetPercentage(0.20f); 
	color = { 0,0,0 }; 
}

UI_Label::~UI_Label()
{
}

void UI_Label::Start()
{
	
}

void UI_Label::Update()
{
	
}

void UI_Label::CleanUp()
{
}

void UI_Label::Draw(bool is_editor)
{
	if(text != "")
		RenderText(); 
}

void UI_Label::RenderText()
{
	bool draw_section = false; 

	if (section.x != -1 && section.y != -1)
		draw_section = true; 

	App->renderer3D->UseUIRenderSettings();

	// Render the rectangle 
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float3 cursor = float3(text_origin.x, text_origin.y, 0);
	int counter = 0;
	int letters_drawn = 0; 

	//Wrapping
	int current_line = 0; 
	float line_distance = 0; 
	float init_offset = text_origin.x; 

	if (App->input->GetKey(SDL_SCANCODE_9) == KEY_UP)
		section += {1, 1}; 

	for (auto it = text_planes.begin(); it != text_planes.end(); it++, counter++)
	{
		if (draw_section && (counter < section.x || counter > section.y))
			continue;
		else
			letters_drawn++; 

		ComponentRectTransform* rtransform = (ComponentRectTransform*)cmp_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);
		ComponentTransform* trans = rtransform->GetTransform();

		// Get Parent Matrix
		float4x4 view_mat = float4x4::identity;

		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		view_mat.Set((float*)matrix);

		// Create increment matrix
		float4x4 increment = float4x4::identity;

		Character* curr_caracter = font.GetCharacter((GLchar)text[counter]);
		Character* next_caracter = nullptr;

		if (counter < text.size() - 1)
			next_caracter = font.GetCharacter((GLchar)text[counter + 1]);
		else
			next_caracter = font.GetCharacter((GLchar)"");

		if (next_caracter == nullptr)
		{
			CONSOLE_ERROR("Trying to pick a null font texture");
			return;
		}

		cursor.x += offset_planes[counter].x;
		cursor.y = text_origin.y + offset_planes[counter].y + -current_line*cmp_container->line_spacing;

		if (draw_section && letters_drawn == 1)
			cursor.x -= offset_planes[counter].x;

		if (counter == 0)
		{
			line_distance = curr_caracter->Size.x / 2.0f;
		}		
		else
			line_distance += offset_planes[counter].x;
				
		if (counter == text_planes.size() - 1)		
			line_distance += curr_caracter->Size.x / 2.0f;
			
		if (line_distance > rtransform->width)
		{
			if(ControlNewLine(cursor, offset_planes, cmp_container->GetClipping(), current_line, counter, text_origin) == false)
				return; 

			line_distance = 0; 	
		}

		increment.SetTranslatePart(cursor);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)(((trans->GetGlobalViewMatrix() * increment).Transposed() * view_mat).v));

		glBindBuffer(GL_ARRAY_BUFFER, (*it)->GetPlane()->GetMesh()->vertices_id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, (*it)->GetPlane()->GetMesh()->uvs_id);
		glBindTexture(GL_TEXTURE_2D, (*it)->GetImgID());
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);

		glColor3f(color.x, color.y, color.z);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*it)->GetPlane()->GetMesh()->indices_id);

		glDrawElements(GL_TRIANGLES, (*it)->GetPlane()->GetMesh()->num_indices, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)view_mat.v);
	}

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void UI_Label::FillTextPlanes()
{	
	for (int i = 0; i < text.size(); i++)	
		CreateCharacterPlane((const char*)text[i], {0,0,0});

	int counter = 0; 
	offset_planes.clear(); 
	for (auto it = text_planes.begin(); it != text_planes.end(); it++, counter++)
	{
		float distancex = 0;
		float distancey = 0;

		Character* curr_character = font.GetCharacter(text[counter]);

		// X offset
		if (counter < text.size() - 1)
			distancex = curr_character->Advance / 2.0 + font.GetCharacter(text[counter + 1])->Advance / 2.0;
		else
			distancex = curr_character->Advance / 2.0f + curr_character->Size.x / 2.0f;

		//Y offset
		float size = (float)curr_character->Size.y;
		float bearingy = (float)curr_character->Bearing.y;
		float center_to_origin = (curr_character->Size.y / 2);
		distancey = -(size - bearingy) + center_to_origin;

		if (counter == 0)
		{
			distancex = 0;
		}
			
		offset_planes.push_back({ distancex, distancey, 0 });
	}		
}

void UI_Label::CreateCharacterPlane(const char * character, float3 position)
{
	// Set proper size
	FT_Load_Char(font.text_font, (GLchar)character, FT_LOAD_RENDER);
	float2 size = { (float)font.text_font->glyph->bitmap.width, (float)font.text_font->glyph->bitmap.rows};

	// Create the corresponding plane 
	UI_Image* new_char_img = new UI_Image(nullptr);
	new_char_img->GetPlane()->InvertImage(size);

	// Get the corresponding texture 
	uint texture_id = font.GetCharacterTexture(character);
	new_char_img->SetImgID(texture_id);

	text_planes.push_back(new_char_img);
}

string UI_Label::GetText() const
{
	return text;
}

float2 UI_Label::GetOrigin() const
{
	return text_origin;
}

void UI_Label::SetOrigin(const float2 new_origin)
{
	text_origin = new_origin;
}

void UI_Label::TranslateOrigin(float2 increment)
{
	text_origin += increment;
}

void UI_Label::SetText(const char * new_text)
{
	text = new_text; 
	text_planes.clear();

	FillTextPlanes();
	
	if(text != "") //Adjust the container plane to the new text size 
		CreateEnclosedPlane(cmp_container->container_plane_vertices);
}

void UI_Label::SetFont(string font_name)
{
	font = App->user_interface->GetFont(font_name);
}

Font UI_Label::GetFont() const
{
	return font;
}

void UI_Label::ResizeFont()
{
	// The new size is already stored in text_size
	std::string font_name = font.name; 	
	App->user_interface->DeleteFont(font.name);
	App->user_interface->LoadNewFont(font_name, text_size); 
	font = App->user_interface->GetFont(font_name);
	SetText(text.c_str());
	cmp_container->SetClipping(cmp_container->GetClipping());
}

bool UI_Label::ControlNewLine(float3& cursor, std::vector<float3>& offset_planes, const ClipTextType clipping_type, int& current_line, const int counter, const float2 init_offset)
{

	ComponentRectTransform* rtransform = (ComponentRectTransform*)cmp_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);

	float total_x_ammount_center = 0; 

	current_line++;
	if (clipping_type == ClipTextType::CLIP_TOPLEFT || clipping_type == ClipTextType::CLIP_TOPRIGHT)
	{
		cursor.x = init_offset.x;
		cursor.y = offset_planes[counter].y + init_offset.y + -current_line * cmp_container->line_spacing;
	}
	else if (clipping_type == ClipTextType::CLIP_CENTER)
	{
		total_x_ammount_center = offset_planes[counter].x;

		cursor.x = -total_x_ammount_center;
		cursor.y = offset_planes[counter].y + -current_line * cmp_container->line_spacing;
	}

	if (cursor.y - VERTICAL_LIMIT_OFFSET <= rtransform->GetRectQuad()->vertices[2].y)
		return false;

	return true; 

}

void UI_Label::CreateEnclosedPlane(float3* points)
{
	// Get Top-Left point
	points[0] = GetContainerPlanePoint(CLIP_TOPLEFT);

	// Get Bottom-Left point
	points[1] = GetContainerPlanePoint(CLIP_TOPRIGHT);

	// Get Top-Right point
	points[2] = GetContainerPlanePoint(CLIP_BOTTOMLEFT);

	// Get Top-Right point
	points[3] = GetContainerPlanePoint(CLIP_BOTTOMRIGHT);
}

float3 UI_Label::GetValueFromRenderedText(const char * point)
{
	float3 return_value = {0,0,0};

	//Get the parent position
	ComponentRectTransform* rtransform = (ComponentRectTransform*)cmp_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);
	float3 g_position = rtransform->GetGlobalPosition();

	if (point == "XMax")
	{
		//It will always lay on the last letter 
		UI_Image* last_img = nullptr; 
		int counter = 0;
		float offset = -1; 

		for (auto it = text_planes.begin(); it != text_planes.end(); it++, counter++)
		{			
			if (counter == text_planes.size() - 1)
			{
				last_img = (*it);
				offset += offset_planes[counter].x;
				break; 
			}	

			offset += offset_planes[counter].x;
		}

		if (last_img == nullptr)
		{
			CONSOLE_LOG("An error has occurred trying to look for the max X"); 
			return { 0,0,0 }; 
		}

		return_value = { last_img->GetPlane()->GetMesh()->vertices[1].x + offset + g_position.x + text_origin.x, 0, 0 };
		return return_value; 
	}
	else if (point == "XMin")
	{
		//It will always lay on the first letter 

		UI_Image* first_img = nullptr;
		int counter = 0;
		float offset = 0;

		auto it = text_planes.begin();	
		first_img = (*it);
			
		if (first_img == nullptr)
		{
			CONSOLE_LOG("An error has occurred trying to look for the min X");
			return { 0,0,0 };
		}

		return_value = { first_img->GetPlane()->GetMesh()->vertices[0].x + g_position.x + text_origin.x, 0, 0 };
		return return_value;
	}
	else if (point == "YMax")
	{
		float max_y = -100000; 
		UI_Image* max_y_plane = nullptr;
		int counter = 0; 

		for (auto it = text_planes.begin(); it != text_planes.end(); it++, counter++)
		{
			float plane_max_y = offset_planes[counter].y;

			if (max_y < plane_max_y)
			{
				max_y = plane_max_y;
				max_y_plane = (*it); 
			}
				
		}

		return_value = { 0, max_y_plane->GetPlane()->GetMesh()->vertices[0].y + max_y + g_position.y + text_origin.y, 0 };
		return return_value;
	}
	else if (point == "YMin")
	{
		float min_y = 100000;
		UI_Image* min_y_plane = nullptr;
		int counter = 0;

		for (auto it = text_planes.begin(); it != text_planes.end(); it++, counter++)
		{
			float plane_min_y = offset_planes[counter].y;

			if (min_y > plane_min_y && plane_min_y != 0) //if plane_min_y == 0 it's an space
			{
				min_y = plane_min_y;
				min_y_plane = (*it); 
			}			
		}

		CONSOLE_LOG("%f", g_position.y); 
		return_value = { 0, min_y_plane->GetPlane()->GetMesh()->vertices[2].y + min_y + g_position.y + text_origin.y, 0 };
		return return_value;
	}

	return return_value;
}

float3 UI_Label::GetContainerPlanePoint(ClipTextType clipping)
{
	float3 ret_point; 

	float3 p1, p2; 

	switch (clipping)
	{
	case ClipTextType::CLIP_TOPLEFT:
		p1 = GetValueFromRenderedText("XMin"); 
		p2 = GetValueFromRenderedText("YMax");
		break; 

	case ClipTextType::CLIP_BOTTOMLEFT:
		p1 = GetValueFromRenderedText("XMin");
		p2 = GetValueFromRenderedText("YMin");
		break;

	case ClipTextType::CLIP_TOPRIGHT:
		p1 = GetValueFromRenderedText("XMax");
		p2 = GetValueFromRenderedText("YMax");
		break;

	case ClipTextType::CLIP_BOTTOMRIGHT:
		p1 = GetValueFromRenderedText("XMax");
		p2 = GetValueFromRenderedText("YMin");
		break;
	}
	
	ret_point = p1 + p2; 
	return ret_point; 
}

void UI_Label::UpdateContainerPlane()
{
	CreateEnclosedPlane(cmp_container->container_plane_vertices);
}

GameObject * UI_Label::GetParentGameObject()
{
	GameObject* go = nullptr; 

	if (cmp_container)
		go = cmp_container->GetGameObject();

	return go;
}

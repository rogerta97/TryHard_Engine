#include "UI_Label.h"
#include "UI_Image.h"
#include "UI_Plane.h"

#include "ComponentText.h"
#include "ComponentTransform.h"
#include "ComponentRectTransform.h"

#include "Application.h"

UI_Label::UI_Label(ComponentText* cmp_text)
{
	cmp_container = cmp_text; 
	SetFont("Antonio-Regular");
	SetText("BCA");
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
	App->renderer3D->UseCurrentRenderSettings();

	// Render the rectangle 
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	float3 cursor = {0,0,0};
	int counter = 0; 

	for (auto it = text_planes.begin(); it != text_planes.end(); it++, counter++)
	{		
		ComponentRectTransform* rtransform = (ComponentRectTransform*)cmp_container->GetGameObject()->GetComponent(CMP_RECTTRANSFORM);
		ComponentTransform* trans = rtransform->GetTransform();

		// Get Parent Matrix
		float4x4 view_mat = float4x4::identity;

		GLfloat matrix[16];
		glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
		view_mat.Set((float*)matrix);

		// Create increment matrix
		float4x4 increment = float4x4::identity;

		increment.SetTranslatePart(cursor); 

		cursor.x += 5;

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)(((increment) * trans->GetGlobalViewMatrix()).Transposed() * view_mat).v);
		
		glBindBuffer(GL_ARRAY_BUFFER, (*it)->GetPlane()->GetMesh()->vertices_id);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		
		glBindBuffer(GL_ARRAY_BUFFER, (*it)->GetPlane()->GetMesh()->uvs_id);
		glBindTexture(GL_TEXTURE_2D, (*it)->GetImgID());
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	
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
	{
		CreateCharacterPlane((const char*)text[i], {0,0,0});
	}
}

void UI_Label::CreateCharacterPlane(const char * character, float3 position)
{
	// Set proper size
	FT_Load_Char(text_font.text_font, (GLchar)character, FT_LOAD_RENDER);
	float2 size = { (float)text_font.text_font->glyph->bitmap.width, (float)text_font.text_font->glyph->bitmap.rows};

	// Create the corresponding plane 
	UI_Image* new_char_img = new UI_Image(nullptr);
	new_char_img->GetPlane()->InvertImage(size);

	// Get the corresponding texture 
	uint texture_id = text_font.GetCharacterTexture(character);
	new_char_img->SetImgID(texture_id);

	text_planes.push_back(new_char_img);
}

string UI_Label::GetText() const
{
	return text;
}

void UI_Label::SetText(const char * new_text)
{
	text = new_text; 
	text_planes.clear();
	FillTextPlanes();
}

void UI_Label::SetFont(string font_name)
{
	text_font = App->user_interface->GetFont(font_name);
}

#include "UI_Label.h"
#include "UI_Plane.h"
#include "ComponentText.h"

#include "Application.h"

UI_Label::UI_Label(ComponentText* cmp_text)
{
	cmp_container = cmp_text; 
	text = "A"; 
	text_size = 0; 
	SetFont("Antonio");
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
	RenderText(); 
}

void UI_Label::RenderText()
{
	char* text_chr = (char*)text.c_str(); 

	for (int i = 0; text_chr[i] != '\0'; i++)
	{
		const char* plane_letter = (const char*)text_chr[i];
		RenderCharacter(plane_letter, { (float)i,0,0 });
	}
}

void UI_Label::RenderCharacter(const char * character, float3 position)
{
	// Create the corresponding plane 
	UI_Plane* new_char_img = new UI_Plane();

	// Get the corresponding texture 
	uint texture_id = text_font.GetCharacterTexture(character);

	App->renderer3D->UseUIRenderSettings();

	// Render the rectangle 
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glBindBuffer(GL_ARRAY_BUFFER, new_char_img->GetMesh()->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, new_char_img->GetMesh()->uvs_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	
	glColor3f(1.0f, 1.0f, 1.0f);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_char_img->GetMesh()->indices_id);
	glDrawElements(GL_TRIANGLES, new_char_img->GetMesh()->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
}

string UI_Label::GetText() const
{
	return text;
}

void UI_Label::SetText(const char * new_text)
{
	text = new_text; 
}

void UI_Label::SetFont(string font_name)
{
	text_font = App->user_interface->GetFont(font_name);

	//if (fnt)
	//{
	//	text_font = *App->user_interface->GetFont(font_name);

	//	if (!text_font.loaded)
	//		text_font.GenerateCharacterList();
	//}

}

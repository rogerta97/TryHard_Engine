#include "UI_Label.h"
#include "ComponentText.h"

#include "Application.h"

UI_Label::UI_Label(ComponentText* cmp_text)
{
	cmp_container = cmp_text; 
	text = "Text"; 
	text_size = 0; 
	SetFont("Antonio-Regular");
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
	FT_Face* fnt = &App->user_interface->GetFont(font_name)->text_font;

	if (fnt)
	{
		text_font = *App->user_interface->GetFont(font_name);

		if (!text_font.loaded)
			text_font.GenerateCharacterList();
	}

}

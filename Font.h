#ifndef _FONT_H_
#define _FONT_H_

#include "ModuleUserInterface.h"
#include "OpenGL.h"
#include <map>
#include <string>

struct Character
{
	Character() {}
	Character(GLuint id, float2 _size, float2 _bearing, GLuint _advance) : TextureID(id), Size(_size), Bearing(_bearing), Advance(_advance) {}
		
	GLuint     TextureID;  // ID handle of the glyph texture
	float2     Size;       // Size of glyph
	float2     Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};


class Font
{
public:
	Font();
	~Font();

	uint GetCharacterTexture(const char* character); 
	Character* GetCharacter(GLchar character) const; 
	void GenerateCharacterList(); 
	void CleanCharacterList(); 

	std::string name; 
	FT_Face text_font; 
	std::map<GLchar, Character*> chars_list;
	int size; 
	bool loaded = false; 

};

#endif


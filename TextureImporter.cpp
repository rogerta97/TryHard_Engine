#include "TextureImporter.h"

#include "DevIL Windows SDK\include\IL\il.h"

#pragma comment(lib, "DevIL Windows SDK/lib/x86/Release/DevIL.lib")
#pragma comment(lib, "DevIL Windows SDK/lib/x86/Release/ILU.lib")
#pragma comment(lib, "DevIL Windows SDK/lib/x86/Release/ILUT.lib")

TextureImporter::TextureImporter()
{
}

bool TextureImporter::Start()
{
	ilInit(); 

	return true;
}

bool TextureImporter::Update()
{
	return true;
}

bool TextureImporter::CleanUp()
{
	return true;
}

Texture* TextureImporter::LoadTexture(const char * path)
{
	Texture* tex; 

	return tex; 
}

TextureImporter::~TextureImporter()
{
}

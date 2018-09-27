#include "TextureImporter.h"

#include "DevIL Windows SDK\include\IL\il.h"
#include "DevIL Windows SDK\include\IL\ilu.h"

#pragma comment(lib, "DevIL Windows SDK/lib/x86/Release/DevIL.lib")
#pragma comment(lib, "DevIL Windows SDK/lib/x86/Release/ILU.lib")
#pragma comment(lib, "DevIL Windows SDK/lib/x86/Release/ILUT.lib")

TextureImporter::TextureImporter()
{
}

bool TextureImporter::Start()
{
	ilInit(); 
	iluInit(); 

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

Texture * TextureImporter::GetCheckedTexture()
{
	Texture* new_tex = new Texture(); 
	new_tex->CreateBuffer(); 
	new_tex->Bind();
	new_tex->SetTextureSettings(); 
	new_tex->SetCheckTexture(); 
	new_tex->UnBind(); 

	return new_tex; 

}

TextureImporter::~TextureImporter()
{
}

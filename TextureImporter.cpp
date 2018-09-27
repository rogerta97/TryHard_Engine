#include "TextureImporter.h"

#include "DevIL Windows SDK\include\IL\il.h"
#include "DevIL Windows SDK\include\IL\ilu.h"
#include "DevIL Windows SDK\include\IL\ilut.h"

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
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

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
	Texture* tex = nullptr;

	ILuint imageID;
	GLuint textureID;
	ILboolean success;
	ILenum error;
	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	success = ilLoadImage(path);

	if (success)
	{
		//Create Texture where data will be stored 
		tex = new Texture(); 

		//Get data of the image
		ILinfo image_info; 
		iluGetImageInfo(&image_info);

		if (image_info.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage(); 

		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success)
		{
			tex->CreateBuffer();

			tex->SetWidth(ilGetInteger(IL_IMAGE_WIDTH));
			tex->SetHeight(ilGetInteger(IL_IMAGE_HEIGHT));
			
			tex->Bind();
			tex->SetTextureSettings();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->GetWidth(), tex->GetHeight(), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			tex->UnBind();

			textures_list.push_back(tex); 
		}		
	}

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

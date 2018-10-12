#pragma once

#include "Mesh.h"
#include "Texture.h"


enum SkyBox_Side
{
	SKYBOX_FRONT,
	SKYBOX_RIGHT,
	SKYBOX_LEFT,
	SKYBOX_BACK,
	SKYBOX_TOP,
	SKYBOX_DOWN

};

class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	
	void InitSkyBox(const char* folder_name); 
	void Draw();

	void CreateFrontPlane(const char* plane_tex_path);
	void CreateRightPlane(const char* plane_tex_path);
	void CreateLeftPlane(const char* plane_tex_path);
	void CreateBackPlane(const char* plane_tex_path);
	void CreateTopPlane(const char* plane_tex_path);
	void CreateDownPlane(const char* plane_tex_path);

	Mesh sky_cube[6]; 
	Texture* sky_textures[6];

};


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

	void CreateFrontPlane(); 
	void CreateRightPlane();
	void CreateLeftPlane(); 
	void CreateBackPlane();
	void CreateTopPlane();
	void CreateDownPlane(); 

	Mesh sky_cube[6]; 
	Texture sky_textures[6];

};


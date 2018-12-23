#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "Mesh.h"
#include "Texture.h"

class ComponentCamera; 
class GameObject; 

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
	
	void InitSkyBox(const char* folder_name, float size); 
	void AttachTo(GameObject* go); 
	void Draw();
	void CleanUpSkyBox(); 

	void CreateFrontPlane(const char* plane_tex_path);
	void CreateRightPlane(const char* plane_tex_path);
	void CreateLeftPlane(const char* plane_tex_path);
	void CreateBackPlane(const char* plane_tex_path);
	void CreateTopPlane(const char* plane_tex_path);
	void CreateDownPlane(const char* plane_tex_path);

	Mesh* sky_cube[6]; 
	Texture* sky_textures[6];
	GameObject* camera_attached; 

	float size; 

};

#endif


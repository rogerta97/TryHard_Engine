#ifndef __UI_IMAGE_H__
#define __UI_IMAGE_H__

#include "UI_Element.h"

class ComponentImage; 
class Material; 
class UI_Plane; 

enum UI_Image_Texture
{
	UI_IMG_TEXTURE,
	UI_IMG_COLOR,
	UI_IMG_TINTED,
};

class UI_Image : public UI_Element
{
public:
	UI_Image(ComponentImage* cmp_container_);
	~UI_Image();

	void Start();
	void Update();
	void CleanUp();
	void Draw(); 

	UI_Plane* GetPlane(); 
	void Resize(float2 size); 

	void CreateDrawSpace(); 
	void DrawImage();

	void SetImgID(uint new_tex);
	uint GetImgID() const;

	void SetMaterial(Material* new_tex); 
	Material* GetMaterial() const; 

	ComponentImage* cmp_container; 

	bool is_glyphy; 
	UI_Image_Texture texture_type;
	float3	  image_color;

private: 

	UI_Plane* plane = nullptr;					//	Plane where the texture (if it has) is going to be displayed
	Material* draw_material = nullptr;			//  Texture that is going to be shown in the image 
	uint	  img_textureID = 0; 
 
};

#endif
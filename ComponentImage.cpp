#include "ComponentImage.h"
#include "ComponentRectTransform.h"
#include "UI_Plane.h"
#include "UI_Image.h"

#include "mmgr\mmgr.h"

ComponentImage::ComponentImage(GameObject* parent)
{
	gameobject = parent; 
	component_type = CMP_IMAGE; 
}

ComponentImage::~ComponentImage()
{
	image->CleanUp();
	delete image; 
}

bool ComponentImage::Start()
{
	image = new UI_Image(this); 
	return false;
}

bool ComponentImage::Update()
{
	if(image)
		image->Update(); 

	return false;
}

bool ComponentImage::CleanUp()
{
	return false;
}

void ComponentImage::Draw(bool is_editor)
{
	image->Draw(); 
}

UI_Image * ComponentImage::GetImage() const
{
	return image;
}

void ComponentImage::FitToRect()
{
	ComponentRectTransform* rtransform = (ComponentRectTransform*)gameobject->GetComponent(CMP_RECTTRANSFORM); 
	image->GetPlane()->Resize(rtransform->width, rtransform->height); 	
}

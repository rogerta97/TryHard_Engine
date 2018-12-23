#ifndef __UI_PLANE_H__
#define __UI_PLANE_H__

#include "UI_Element.h"

//Base element that can not be instantiated by itself, used for ease development. Equals to an image without texture.  

class UI_Plane : public UI_Element
{
public:
	UI_Plane(float2 size);
	UI_Plane();
	~UI_Plane();

	void Start();
	void Update();
	void CleanUp();

	float2 GetSize() const; 

	void CreatePlaneMesh(float2 size = {1,1});
	void InvertImage(float2 size = { 1,1 });
	void Resize(float x, float y); 

	Mesh* GetMesh() const; 
	void SetMesh(Mesh* nValue); 

private:

	Mesh * quad_plane_mesh; 
};

#endif


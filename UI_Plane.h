#pragma once
#include "UI_Element.h"

//Base element that can not be instantiated by itself, used for ease development. Equals to an image without texture.  

class UI_Plane : public UI_Element
{
public:
	UI_Plane();
	~UI_Plane();

	void Start();
	void Update();
	void CleanUp();

	void CreatePlaneMesh();

	Mesh* GetMesh() const; 
	void SetMesh(Mesh* nValue); 

private:

	Mesh * quad_plane_mesh; 
};


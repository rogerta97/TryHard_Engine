#pragma once

#include "Component.h"
#include "Mesh.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	bool Update();

	void SetMesh(Mesh* new_mesh);

private:

	Mesh * mesh;
	
	bool draw_mesh; 
};


#include "ComponentBoundingBox.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "OpenGL.h"

#include "Mesh.h"

ComponentBoundingBox::ComponentBoundingBox()
{
	component_type = CMP_BOUNDINGBOX; 
	active = true;
}

ComponentBoundingBox::~ComponentBoundingBox()
{
}

bool ComponentBoundingBox::Start()
{
	CreateEnclosedMeshAABB(); 
	color.Set(0, 1, 0, 1);

	return true;
}

bool ComponentBoundingBox::Update()
{
	//Get the points 
	if (gameobject->selected)
	{
		LineSegment curr_line;

		glBegin(GL_LINES);
		glColor3f(color.r, color.g, color.b);

		for (int i = 0; i < 12; i++)
		{
			curr_line = bounding_box.Edge(i);

			glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
			glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);
		}

		glEnd();
	}

	return true;
}

bool ComponentBoundingBox::CleanUp()
{
	return true;
}


bool ComponentBoundingBox::CreateEnclosedMeshAABB()
{
	//Get Vertices of the mesh 
	ComponentMesh* mesh_cmp = (ComponentMesh*)gameobject->GetComponent(CMP_RENDERER);
	
	if (mesh_cmp != nullptr)
	{
		bounding_box.SetNegativeInfinity();
		bounding_box = bounding_box.MinimalEnclosingAABB(mesh_cmp->GetMesh()->vertices, mesh_cmp->GetMesh()->num_vertices);
		return true; 
	}
	

	return false;
}

#include "Mesh.h"
#include "OpenGL.h"
#include "MathGeoLib\MathGeoLib.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

uint Mesh::CreateBuffer()
{
	uint new_buffer; 
	glGenBuffers(1, (GLuint*)&new_buffer);
	return new_buffer;
}

bool Mesh::SetCubeData()
{

	return false;
}

bool Mesh::SetPlaneData()
{
	//Create the buffers
	vertex_id = CreateBuffer(); 
	indices_id = CreateBuffer(); 

	type = BasicMeshType::MESH_PLANE;
		
	//Create Vertices
	num_vertex = 4;
	vertex = new float3[num_vertex];

	vertex[0].x = 1.0f;
	vertex[0].y = 0.0f;
	vertex[0].z = 1.0f;

	vertex[1].x = -1.0f;
	vertex[1].y = 0.0f;
	vertex[1].z = 1.0f;

	vertex[2].x = 1.0;
	vertex[2].y = 0.0f;
	vertex[2].z = -1.0f;

	vertex[2].x = -1.0;
	vertex[2].y = 0.0f;
	vertex[2].z = -1.0f;

	glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	//Pass to VRAM
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex*3, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Create Indices
	num_indices = 6;
	indices = new int[num_indices];
	indices[0] = 1;
	indices[1] = 0;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 1;
	indices[5] = 2;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	//Pass to VRAM
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true; 
}

void Mesh::DrawMesh()
{

	glEnableClientState(GL_VERTEX_ARRAY);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertex_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glDrawElements(GL_TRIANGLES, num_indices, GL_INT, NULL);
	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_VERTEX_ARRAY, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);

	//const float sizex = 0.5f;
	//const float sizey = 0.5f;
	//const float sizez = 0.5f;

	//glBegin(GL_QUADS);

	//glColor3f(0.5, 0.6, 1.0);

	//// FRONT
	//glVertex3f(-sizex, -sizey, sizez);
	//glVertex3f(sizex, -sizey, sizez);
	//glVertex3f(sizex, sizey, sizez);
	//glVertex3f(-sizex, sizey, sizez);

	//// BACK
	//glVertex3f(-sizex, -sizey, -sizez);
	//glVertex3f(-sizex, sizey, -sizez);
	//glVertex3f(sizex, sizey, -sizez);
	//glVertex3f(sizex, -sizey, -sizez);

	//glColor3f(0.5, 0.6, 1.0);

	//// LEFT
	//glVertex3f(-sizex, -sizey, sizez);
	//glVertex3f(-sizex, sizey, sizez);
	//glVertex3f(-sizex, sizey, -sizez);
	//glVertex3f(-sizex, -sizey, -sizez);

	//// RIGHT
	//glVertex3f(sizex, -sizey, -sizez);
	//glVertex3f(sizex, sizey, -sizez);
	//glVertex3f(sizex, sizey, sizez);
	//glVertex3f(sizex, -sizey, sizez);

	//glColor3f(0.5, 0.6, 1.0);

	//// TOP
	//glVertex3f(-sizex, sizey, sizez);
	//glVertex3f(sizex, sizey, sizez);
	//glVertex3f(sizex, sizey, -sizez);
	//glVertex3f(-sizex, sizey, -sizez);

	//// BOTTOM
	//glVertex3f(-sizex, -sizey, sizez);
	//glVertex3f(-sizex, -sizey, -sizez);
	//glVertex3f(sizex, -sizey, -sizez);
	//glVertex3f(sizex, -sizey, sizez);

	//glEnd();

}

BasicMeshType Mesh::GetType()
{
	return type;
}

void Mesh::UnbindBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

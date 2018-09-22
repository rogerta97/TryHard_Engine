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
	glGenBuffers(1, (GLuint*)&buffer_id);
	return buffer_id; 
}

void Mesh::BindBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
}

bool Mesh::SetCubeData()
{

	return false;
}

bool Mesh::SetPlaneData()
{
	if (buffer_id == 0){
		CONSOLE_ERROR("Buffer ID == 0 and data can't be assigned");
		return false; 
	}

	type = BasicMeshType::MESH_PLANE;
		
	//Generate Data 
	float3 vertex_[4]; 
	vertex_[0] = { 1.0f, 0.0f, 1.0f };
	vertex_[1] = { -1.0f, 0.0f, 1.0f };
	vertex_[2] = { 1.0f, 0.0f, -1.0f };
	vertex_[3] = { -1.0f, 0.0f, -1.0f };

	int index[6] = { 1, 0, 2, 3, 1, 2 }; 

	vertex = vertex_; 
	indices = index; 

	num_vertex = 4;
	num_index = 6;

	//Assign it to VRAM
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex * 3, vertex, GL_STATIC_DRAW);

	return true; 
}

void Mesh::DrawMesh()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	// ... draw other buffers

	glDrawArrays(GL_TRIANGLES, 0, num_vertex * 3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

BasicMeshType Mesh::GetType()
{
	return type;
}

void Mesh::UnbindBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

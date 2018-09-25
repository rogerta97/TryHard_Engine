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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);
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
	vertex = new float3[3];
	vertex[0] = { 1.0f, 0.0f, 1.0f };
	vertex[1] = { -1.0f, 0.0f, 1.0f };
	vertex[2] = { 1.0f, 0.0f, -1.0f };
	//vertex[3] = { -1.0f, 0.0f, -1.0f };

	indices = new int[3];
	indices[0] = { 1};
	indices[1] = { 0 };
	indices[2] = { 2 };
	//indices[3] = { 0 };
	//indices[4] = { 0};
	//indices[5] = { 0 };

	num_vertex = 3;
	num_index = 3;

	//Assign it to VRAM
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*num_index, indices, GL_STATIC_DRAW);

	return true; 
}

void Mesh::DrawMesh()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id);

	// ... draw other buffers
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

BasicMeshType Mesh::GetType()
{
	return type;
}

void Mesh::UnbindBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

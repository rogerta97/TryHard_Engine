#include "Mesh.h"
#include "OpenGL.h"
#include "MathGeoLib\MathGeoLib.h"

Mesh::Mesh()
{
	name = ""; 
	color = { DEFAULT_GEOMETRY_COLOR };
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
	////Create the buffers
	//vertex_id = CreateBuffer();
	//indices_id = CreateBuffer();

	//type = BasicMeshType::MESH_CUBE;
	//num_vertex = 8;
	//vertex = new float3[num_vertex];

	//int w = 1;
	//int h = 1;
	//int d = 1;

	//vertex[0].x = -w;
	//vertex[0].y = -h;
	//vertex[0].z = +d;

	//vertex[1].x = +w;
	//vertex[1].y = -h;
	//vertex[1].z = +d;

	//vertex[2].x = +w;
	//vertex[2].y = +h;
	//vertex[2].z = +d;

	//vertex[3].x = +w;
	//vertex[3].y = +h;
	//vertex[3].z = +d;

	//vertex[4].x = -w;
	//vertex[4].y = -h;
	//vertex[4].z = -d;

	//vertex[5].x = +w;
	//vertex[5].y = -h;
	//vertex[5].z = -d;

	//vertex[6].x = +w;
	//vertex[6].y = +h;
	//vertex[6].z = -d;

	//vertex[7].x = +w;
	//vertex[7].y = +h;
	//vertex[7].z = -d;

	//glBindBuffer(GL_ARRAY_BUFFER, vertex_id);
	////Pass to VRAM
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertex * 3, vertex, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//////Create Indices
	//num_indices = 6;
	//indices = new int[num_indices];
	//indices[0] = 0;
	//indices[1] = 1;
	//indices[2] = 2;
	//indices[3] = 3;
	//indices[4] = 2;
	//indices[5] = 1;

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	////Pass to VRAM
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*num_indices, indices, GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return false;
}

bool Mesh::SetPlaneData()
{
	//Create the buffers
	vertices_id = CreateBuffer(); 
	indices_id = CreateBuffer(); 

	type = BasicMeshType::MESH_PLANE;
		
	//Create Vertices
	num_vertices = 4;
	vertices = new float3[num_vertices];

	vertices[0].x = 1.0f;
	vertices[0].y = 0.0f;
	vertices[0].z = 1.0f;

	vertices[1].x = 1.0;
	vertices[1].y = 0.0f;
	vertices[1].z = -1.0f;

	vertices[2].x = -1.0f;
	vertices[2].y = 0.0f;
	vertices[2].z = 1.0f;

	vertices[3].x = -1.0;
	vertices[3].y = 0.0f;
	vertices[3].z = -1.0f;

	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	//Pass to VRAM
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices *3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	////Create Indices
	num_indices = 6;
	indices = new int[num_indices];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 1;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	//Pass to VRAM
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true; 
}



BasicMeshType Mesh::GetType()
{
	return type;
}

void Mesh::UnbindBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

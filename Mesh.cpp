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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices*3, vertices, GL_STATIC_DRAW); //Info to VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Create Indices
	num_indices = 6;
	indices = new int[num_indices];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 1;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*num_indices, indices, GL_STATIC_DRAW); //Info to VRAM
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Create Texture Coords
	num_uvs = num_indices; 
	uvs_cords = new float[num_uvs*2]; 

	uvs_cords[0] = 0.0f;
	uvs_cords[1] = 1.0f;

	uvs_cords[2] = 0.0f;
	uvs_cords[3] = 0.0f;

	uvs_cords[4] = 1.0f;
	uvs_cords[5] = 1.0f;

	uvs_cords[6] = 1.0f;
	uvs_cords[7] = 0.0f;

	uvs_cords[8] = 1.0f;
	uvs_cords[9] = 1.0f;

	uvs_cords[10] = 0.0f;
	uvs_cords[11] = 0.0f;

	glGenBuffers(1, &uvs_id);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_uvs *2, uvs_cords, GL_STATIC_DRAW); //Info to VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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

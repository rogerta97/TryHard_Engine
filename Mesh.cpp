#include "Mesh.h"
#include "OpenGL.h"
#include "Resource.h"
#include "MathGeoLib\MathGeoLib.h"
#include <iostream>
#include <fstream>

#include "Application.h"


#include "mmgr\mmgr.h"


Mesh::Mesh()
{
	name = ""; 
	color = { DEFAULT_GEOMETRY_COLOR };

	num_normals = num_vertices = num_indices = num_uvs =  0; 
	vertices_id = normals_id = indices_id = uvs_id = 0;

	reference_counting = 0;
	meta_path = "";

	load_buffer = nullptr;
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
	//Set Vertices
	type = BasicMeshType::MESH_CUBE;

	num_vertices = 8;
	vertices = new float3[num_vertices];

	int size = 2;
	float half_size = (float)size / 2.0f; 

	vertices[0] = { -half_size, half_size, half_size };
	vertices[1] = { half_size, half_size, half_size };
	vertices[2] = { -half_size, -half_size, half_size };
	vertices[3] = { half_size, -half_size, half_size };

	vertices[4] = { -half_size, half_size, -half_size };
	vertices[5] = { half_size, half_size, -half_size };
	vertices[6] = { -half_size, -half_size, -half_size };
	vertices[7] = { half_size, -half_size, -half_size };

	//Create Indices
	num_indices = 36;
	indices = new int[num_indices];

	//Front
	indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;

	indices[3] = 2;
	indices[4] = 3;
	indices[5] = 1;

	//Back
	indices[6] = 7;
	indices[7] = 6;
	indices[8] = 4;

	indices[9] = 4;
	indices[10] = 5;
	indices[11] = 7;

	// Right
	indices[12] = 7;
	indices[13] = 1;
	indices[14] = 3;

	indices[15] = 7;
	indices[16] = 5;
	indices[17] = 1;

	// Left
	indices[18] = 6;
	indices[19] = 0;
	indices[20] = 4;

	indices[21] = 2;
	indices[22] = 0;
	indices[23] = 6;

	// Top
	indices[24] = 1;
	indices[25] = 5;
	indices[26] = 0;

	indices[27] = 0;
	indices[28] = 5;
	indices[29] = 4;

	// Bottom
	indices[30] = 3;
	indices[31] = 2;
	indices[32] = 7;

	indices[33] = 2;
	indices[34] = 6;
	indices[35] = 7;

	normal_cords = new float3[num_vertices];
	num_normals = num_vertices; 

	normal_cords[0] = vertices[0]; normal_cords[1] = vertices[1];
	normal_cords[2] = vertices[2]; normal_cords[3] = vertices[3];
	normal_cords[4] = vertices[4]; normal_cords[5] = vertices[5];
	normal_cords[6] = vertices[6]; normal_cords[7] = vertices[7];

	////Create UV's
	uvs_id = CreateBuffer();

	num_uvs = num_indices;
	uvs_cords = new float[num_uvs*2];

	//Front 
	uvs_cords[0] = 1.0f; 
	uvs_cords[1] = 1.0f;

	uvs_cords[2] = 0.0f;
	uvs_cords[3] = 0.0f;

	uvs_cords[4] = 1.0f;
	uvs_cords[5] = 0.0f;

	return true;
}

bool Mesh::SetVertPlaneData(float2 size)
{
	//Set Vertices
	type = BasicMeshType::MESH_CUBE;

	num_vertices = 4;
	vertices = new float3[num_vertices];

	float2 half_size = (float2)size / 2.0f;

	vertices[0] = { -half_size.x, half_size.y, 0 };
	vertices[1] = { half_size.x, half_size.y, 0 };
	vertices[2] = { -half_size.x, -half_size.y, 0 };
	vertices[3] = { half_size.x, -half_size.y, 0 };

	num_indices = 6;
	indices = new int[num_indices];

	//Front
	indices[0] = 0;
	indices[1] = 2;
	indices[2] = 1;

	indices[3] = 1;
	indices[4] = 2;
	indices[5] = 3;

	num_uvs = num_vertices;
	uvs_cords = new float[num_uvs * 3];

	//Front 
	uvs_cords[0] = 0.0f;
	uvs_cords[1] = 1.0f;
	uvs_cords[2] = 0.0f;

	uvs_cords[3] = 1.0f;
	uvs_cords[4] = 1.0f;
	uvs_cords[5] = 0.0f;

	uvs_cords[6] = 0.0f;
	uvs_cords[7] = 0.0f;
	uvs_cords[8] = 0.0f;

	uvs_cords[9] = 1.0f;
	uvs_cords[10] = 0.0f;
	uvs_cords[11] = 0.0f;

	return true;
}

float3 Mesh::CenterVertices(float3* vertices, int num)
{
	float3 min = float3::inf;
	float3 max = -float3::inf;
	for (int i = 0; i < num; i++)
	{
		if (vertices[i].x > max.x) max.x = vertices[i].x;
		if (vertices[i].y > max.y) max.y = vertices[i].y;
		if (vertices[i].z > max.z) max.z = vertices[i].z;

		if (vertices[i].x < min.x) min.x = vertices[i].x;
		if (vertices[i].y < min.y) min.y = vertices[i].y;
		if (vertices[i].z < min.z) min.z = vertices[i].z;
	}

	for (int i = 0; i < num; i++)
		vertices[i] -= (max + min) / 2;
	
	return(max + min) / 2;
}

void Mesh::CleanMeshData()
{	
	if (reference_counting == 0)
	{
		if (vertices_id != 0)
		{
			delete[] vertices;
			glDeleteBuffers(1, &vertices_id);
			
			vertices_id = 0; 
		}

		if (indices_id != 0)
		{
			delete[] indices;
			glDeleteBuffers(1, &indices_id);
			
			indices_id = 0; 
		}

		if (uvs_id != 0)
		{
			delete[] uvs_cords;
			glDeleteBuffers(1, &uvs_id);			
			indices_id = 0;
		}
	}

	//App->resources->DeleteFromList(uid);
	
}



BasicMeshType Mesh::GetType() const
{
	return type;
}

bool Mesh::IsLoadedToMemory() const
{
	if(vertices_id != 0 && indices_id != 0)
		return true;

	return false; 
}

void Mesh::LoadToMemory()
{
	if (num_vertices != 0)
	{
		glGenBuffers(1, &vertices_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (num_indices != 0)
	{
		glGenBuffers(1, &indices_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*num_indices, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if (num_uvs != 0)
	{
		glGenBuffers(1, &uvs_id);
		glBindBuffer(GL_ARRAY_BUFFER, uvs_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_uvs * 3, uvs_cords, GL_STATIC_DRAW); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	if (num_normals != 0)
	{
		glGenBuffers(1, &normals_id);
		glBindBuffer(GL_ARRAY_BUFFER, normals_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*num_normals, normal_cords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
}


void Mesh::UnbindBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

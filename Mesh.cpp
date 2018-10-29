#include "Mesh.h"
#include "OpenGL.h"
#include "Resource.h"
#include "MathGeoLib\MathGeoLib.h"
#include <iostream>
#include <fstream>

#include "Application.h"

Mesh::Mesh()
{
	name = ""; 
	color = { DEFAULT_GEOMETRY_COLOR };

	num_normals = num_vertices = num_indices = num_uvs = 0; 
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

	//uvs_cords[6] = 1.0f; uvs_cords[7] = 1.0f;
	//uvs_cords[8] = 0.0f; uvs_cords[9] = 1.0f;
	//uvs_cords[10] = 0.0f; uvs_cords[11] = 0.0f;

	//uvs_cords[6] = { 1.0f, 0.0f, 0.0f };	uvs_cords[7] = { 1.0f, 0.0f, 0.0f};
	//uvs_cords[8] = { 0.0f, 1.0f, 0.0f };	uvs_cords[9] = { 1.0f, 0.0f, 0.0f};
	//uvs_cords[10] = { 1.0f, 1.0f, 0.0f };	uvs_cords[11] = { 0.0f, 1.0f, 0.0f };

	//uvs_cords[12] = { 0.0f, 0.0f, 0.0f };	uvs_cords[13] = { 1.0f, 1.0f, 0.0f };
	//uvs_cords[14] = { 1.0f, 0.0f, 0.0f };	uvs_cords[15] = { 0.0f, 0.0f, 0.0f };
	//uvs_cords[16] = { 0.0f, 1.0f, 0.0f };	uvs_cords[17] = { 1.0f, 1.0f, 0.0f };

	//uvs_cords[18] = { 0.0f, 0.0f, 0.0f };	uvs_cords[19] = { 1.0f, 1.0f, 0.0f };
	//uvs_cords[20] = { 1.0f, 0.0f, 0.0f };	uvs_cords[21] = { 0.0f, 1.0f, 0.0f };
	//uvs_cords[22] = { 1.0f, 1.0f, 0.0f };	uvs_cords[23] = { 0.0f, 0.0f, 0.0f };

	//uvs_cords[24] = { 1.0f, 0.0f, 0.0f };	uvs_cords[25] = { 0.0f, 0.0f, 0.0f };
	//uvs_cords[26] = { 1.0f, 1.0f, 0.0f };	uvs_cords[27] = { 0.0f, 0.0f, 0.0f };
	//uvs_cords[28] = { 0.0f, 1.0f, 0.0f };	uvs_cords[29] = { 1.0f, 0.0f, 0.0f };

	//uvs_cords[30] = { 0.0f, 0.0f, 0.0f };	uvs_cords[31] = { 1.0f, 1.0f, 0.0f };
	//uvs_cords[32] = { 1.0f, 0.0f, 0.0f };	uvs_cords[33] = { 0.0f, 0.0f, 0.0f };
	//uvs_cords[34] = { 0.0f, 1.0f, 0.0f };	uvs_cords[35] = { 1.0f, 1.0f, 0.0f };

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

//bool Mesh::SetPlaneData()
//{
//	//Create the buffers
//	type = BasicMeshType::MESH_PLANE;
//		
//	//Create Vertices
//	num_vertices = 4;
//	vertices = new float3[num_vertices];
//
//	vertices[0].x = 1.0f;
//	vertices[0].y = 0.0f;
//	vertices[0].z = 1.0f;
//
//	vertices[1].x = 1.0;
//	vertices[1].y = 0.0f;
//	vertices[1].z = -1.0f;
//
//	vertices[2].x = -1.0f;
//	vertices[2].y = 0.0f;
//	vertices[2].z = 1.0f;
//
//	vertices[3].x = -1.0;
//	vertices[3].y = 0.0f;
//	vertices[3].z = -1.0f;
//
//	////Create Indices
//	num_indices = 6;
//	indices = new int[num_indices];
//	indices[0] = 0;
//	indices[1] = 1;
//	indices[2] = 2;
//	indices[3] = 3;
//	indices[4] = 2;
//	indices[5] = 1;
//
//	num_uvs = num_indices;
//	uvs_cords = new float[num_uvs*2];
//
//	uvs_cords[0] = 0.0f;
//	uvs_cords[1] = 1.0f;
//
//	uvs_cords[2] = 0.0f;
//	uvs_cords[3] = 0.0f;
//
//	uvs_cords[4] = 1.0f;
//	uvs_cords[5] = 1.0f;
//
//	uvs_cords[6] = 1.0f;
//	uvs_cords[7] = 0.0f;
//
//	uvs_cords[8] = 1.0f;
//	uvs_cords[9] = 1.0f;
//
//	uvs_cords[10] = 0.0f;
//	uvs_cords[11] = 0.0f;
//
//	return true; 
//}
//
//bool Mesh::SetSphereData()
//{
//	////Create the buffers
//	//vertices_id = CreateBuffer();
//	//indices_id = CreateBuffer();
//
//	//std::vector<float3> sphere;
//
//	////---sphere
//	//float radius = 1;
//	//float sectors = 10;
//	//float stacks = 10;
//	//vec position(0, 0, 0);
//
//	//float x, y, z, xy;                              // vertex position
//
//	//float sectorStep = 2 * pi / sectors;
//	//float stackStep = pi / stacks;
//	//float sectorAngle, stackAngle;
//
//	//for (int i = 0; i <= stacks; ++i)
//	//{
//	//	stackAngle = pi / 2 - i * stackStep;        // starting from pi/2 to -pi/2
//	//	xy = radius * cosf(stackAngle);             // r * cos(u)
//	//	y = position.y + radius * sinf(stackAngle);              // r * sin(u)
//
//	//															 // add (sectorCount+1) vertices per stack
//	//															 // the first and last vertices have same position and normal, but different tex coods
//	//	for (int j = 0; j <= sectors; ++j)
//	//	{
//	//		sectorAngle = j * sectorStep;
//
//	//		// vertex position (x, y, z)
//	//		z = position.z + xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
//	//		x = position.x + xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
//	//		sphere.push_back(vec(x, y, z));
//
//	//	}
//	//}
//
//	//glGenBuffers(1, (GLuint*) &(buffsphereID));
//	//glBindBuffer(GL_ARRAY_BUFFER, buffsphereID);
//	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphere.size() * 3, &sphere[0], GL_STATIC_DRAW);
//
//
//
//	//int k1, k2;
//	//for (int i = 0; i < stacks; ++i)
//	//{
//	//	k1 = i * (sectors + 1);     // beginning of current stack
//	//	k2 = k1 + sectors + 1;      // beginning of next stack
//
//	//	for (int j = 0; j < sectors; ++j, ++k1, ++k2)
//	//	{
//	//		// 2 triangles per sector excluding 1st and last stacks
//	//		if (i != 0)
//	//		{
//	//			sphereIndices.push_back(k1);
//	//			sphereIndices.push_back(k2);
//	//			sphereIndices.push_back(k1 + 1);
//	//		}
//
//	//		if (i != (stacks - 1))
//	//		{
//	//			sphereIndices.push_back(k1 + 1);
//	//			sphereIndices.push_back(k2);
//	//			sphereIndices.push_back(k2 + 1);
//	//		}
//	//	}
//	//}
//
//	//glGenBuffers(1, (GLuint*)&(buffIndicesSphereID));
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffIndicesSphereID);
//	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * sphereIndices.size(), &sphereIndices[0], GL_STATIC_DRAW);
//
//	//GenBuffFromMeshes();
//
//	return true;
//}

void Mesh::CleanMeshData()
{	
	//if (num_vertices != 0)
	//{
	//	glDeleteBuffers(1, &vertices_id);
	//	//delete(vertices);
	//}

	//if (num_indices != 0)
	//{
	//	glDeleteBuffers(1, &indices_id);
	//	//delete(indices);
	//}

	//if (num_uvs != 0)
	//{
	//	glDeleteBuffers(1, &uvs_id);
	//	//delete(uvs_cords);
	//}

	//if (num_normals != 0)
	//{
	//	glDeleteBuffers(1, &normals_id);
	//	//delete(normal_cords);
	//}
}



BasicMeshType Mesh::GetType() const
{
	return type;
}

void Mesh::LoadToMemory()
{
	if (num_vertices != 0)
	{
		glGenBuffers(1, &vertices_id);
		glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CONSOLE_DEBUG("Vertices from '%s' Loaded To Memory with ID: %d", name.c_str(), vertices_id);
	}

	if (num_indices != 0)
	{
		glGenBuffers(1, &indices_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*num_indices, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		CONSOLE_DEBUG("Indices from '%s' Loaded To Memory with ID: %d", name.c_str(), indices_id);
	}

	if (num_uvs != 0)
	{
		glGenBuffers(1, &uvs_id);
		glBindBuffer(GL_ARRAY_BUFFER, uvs_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_uvs * 3, uvs_cords, GL_STATIC_DRAW); 
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CONSOLE_DEBUG("UVs from '%s' Loaded To Memory with ID: %d", name.c_str(), uvs_id);
	}

	if (num_normals != 0)
	{
		glGenBuffers(1, &normals_id);
		glBindBuffer(GL_ARRAY_BUFFER, normals_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float3)*num_normals, normal_cords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CONSOLE_DEBUG("Normals from '%s' Loaded To Memory with ID: %d", name.c_str(), normals_id);
	}
	
}


void Mesh::UnbindBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

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
	//Create the buffers
	vertices_id = CreateBuffer();
	indices_id = CreateBuffer();

	type = BasicMeshType::MESH_CUBE;
	num_vertices = 8;
	vertices = new float3[num_vertices];

	int w = 1;
	int h = 1;
	int d = 1;

	vertices[0].x = -w;
	vertices[0].y = -h;
	vertices[0].z = +d;

	vertices[1].x = +w;
	vertices[1].y = -h;
	vertices[1].z = +d;

	vertices[2].x = -w;
	vertices[2].y = +h;
	vertices[2].z = +d;

	vertices[3].x = +w;
	vertices[3].y = +h;
	vertices[3].z = +d;

	vertices[4].x = -w;
	vertices[4].y = -h;
	vertices[4].z = -d;

	vertices[5].x = +w;
	vertices[5].y = -h;
	vertices[5].z = -d;

	vertices[6].x = -w;
	vertices[6].y = +h;
	vertices[6].z = -d;

	vertices[7].x = +w;
	vertices[7].y = +h;
	vertices[7].z = -d;

	glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
	//Pass to VRAM
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_vertices * 3, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	////Create Indices
	num_indices = 36;
	indices = new int[num_indices];

	//Front
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 1;

	//Right
	indices[6] = 1;
	indices[7] = 5;
	indices[8] = 3;

	indices[9] = 5;
	indices[10] = 7;
	indices[11] = 3;

	//Back
	indices[12] = 4;
	indices[13] = 7;
	indices[14] = 5;

	indices[15] = 4;
	indices[16] = 6;
	indices[17] = 7;

	//Left
	indices[18] = 0;
	indices[19] = 6;
	indices[20] = 4;

	indices[21] = 2;
	indices[22] = 6;
	indices[23] = 0;

	//Top
	indices[24] = 2;
	indices[25] = 3;
	indices[26] = 6;

	indices[27] = 3;
	indices[28] = 7;
	indices[29] = 6;

	//Bottom
	indices[30] = 0;
	indices[31] = 5;
	indices[32] = 1;

	indices[33] = 0;
	indices[34] = 4;
	indices[35] = 5;


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
	//Pass to VRAM
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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

	num_uvs = num_indices;
	uvs_cords = new float[num_uvs * 3];

	uvs_cords[0] = 0.0f;
	uvs_cords[1] = 1.0f;
	uvs_cords[2] = 0.0f;

	uvs_cords[3] = 0.0f;
	uvs_cords[4] = 0.0f;
	uvs_cords[5] = 0.0f;

	uvs_cords[6] = 1.0f;
	uvs_cords[7] = 1.0f;
	uvs_cords[8] = 0.0f;

	uvs_cords[9] = 1.0f;
	uvs_cords[10] = 0.0f;
	uvs_cords[11] = 0.0f;

	uvs_cords[12] = 1.0f;
	uvs_cords[13] = 1.0f;
	uvs_cords[14] = 0.0f;

	uvs_cords[15] = 0.0f;
	uvs_cords[16] = 0.0f;
	uvs_cords[17] = 0.0f;

	glGenBuffers(1, &uvs_id);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*num_uvs * 3, uvs_cords, GL_STATIC_DRAW); //Info to VRAM
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true; 
}

bool Mesh::SetSphereData()
{
	////Create the buffers
	//vertices_id = CreateBuffer();
	//indices_id = CreateBuffer();

	//std::vector<float3> sphere;

	////---sphere
	//float radius = 1;
	//float sectors = 10;
	//float stacks = 10;
	//vec position(0, 0, 0);

	//float x, y, z, xy;                              // vertex position

	//float sectorStep = 2 * pi / sectors;
	//float stackStep = pi / stacks;
	//float sectorAngle, stackAngle;

	//for (int i = 0; i <= stacks; ++i)
	//{
	//	stackAngle = pi / 2 - i * stackStep;        // starting from pi/2 to -pi/2
	//	xy = radius * cosf(stackAngle);             // r * cos(u)
	//	y = position.y + radius * sinf(stackAngle);              // r * sin(u)

	//															 // add (sectorCount+1) vertices per stack
	//															 // the first and last vertices have same position and normal, but different tex coods
	//	for (int j = 0; j <= sectors; ++j)
	//	{
	//		sectorAngle = j * sectorStep;

	//		// vertex position (x, y, z)
	//		z = position.z + xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
	//		x = position.x + xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
	//		sphere.push_back(vec(x, y, z));

	//	}
	//}

	//glGenBuffers(1, (GLuint*) &(buffsphereID));
	//glBindBuffer(GL_ARRAY_BUFFER, buffsphereID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*sphere.size() * 3, &sphere[0], GL_STATIC_DRAW);



	//int k1, k2;
	//for (int i = 0; i < stacks; ++i)
	//{
	//	k1 = i * (sectors + 1);     // beginning of current stack
	//	k2 = k1 + sectors + 1;      // beginning of next stack

	//	for (int j = 0; j < sectors; ++j, ++k1, ++k2)
	//	{
	//		// 2 triangles per sector excluding 1st and last stacks
	//		if (i != 0)
	//		{
	//			sphereIndices.push_back(k1);
	//			sphereIndices.push_back(k2);
	//			sphereIndices.push_back(k1 + 1);
	//		}

	//		if (i != (stacks - 1))
	//		{
	//			sphereIndices.push_back(k1 + 1);
	//			sphereIndices.push_back(k2);
	//			sphereIndices.push_back(k2 + 1);
	//		}
	//	}
	//}

	//glGenBuffers(1, (GLuint*)&(buffIndicesSphereID));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffIndicesSphereID);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * sphereIndices.size(), &sphereIndices[0], GL_STATIC_DRAW);

	//GenBuffFromMeshes();

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

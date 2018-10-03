#pragma once

#include "MathGeoLib\MathGeoLib.h"
#include "Globals.h"

enum BasicMeshType { MESH_CUBE, MESH_PLANE, MESH_FBX };

class Mesh
{
public:
	Mesh();
	~Mesh();

	uint CreateBuffer();
	void UnbindBuffer();

	bool SetCubeData(); 
	bool SetPlaneData(); 

	BasicMeshType GetType();

public: 

	std::string name; 
	float3 color; 

	float3* vertices; 
	int* indices;
	float* uvs_cords; 

	uint num_vertices;
	uint num_indices; 
	uint num_uvs; 

	uint vertices_id; 
	uint indices_id; 
	uint uvs_id; 

	BasicMeshType type; 
};


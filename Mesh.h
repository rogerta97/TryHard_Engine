#pragma once

#include "MathGeoLib\MathGeoLib.h"
#include "Globals.h"

enum BasicMeshType { MESH_CUBE, MESH_PLANE, MESH_FBX, MESH_SPHERE };

class Mesh
{
public:
	Mesh();
	~Mesh();

	uint CreateBuffer();
	void UnbindBuffer();

	bool SetCubeData(); 
	bool SetPlaneData(); 
	bool SetSphereData();

	BasicMeshType GetType();

public: 

	std::string name; 
	float3 color; 

	float3* vertices; 
	int* indices;
	float* uvs_cords;
	float3* normal_cords; 


	uint num_vertices;
	uint num_indices; 
	uint num_uvs;
	uint num_normals; 

	uint vertices_id; 
	uint indices_id; 
	uint uvs_id; 
	uint normals_id; 

	BasicMeshType type; 
};


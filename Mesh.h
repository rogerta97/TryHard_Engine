#pragma once

#include "MathGeoLib\MathGeoLib.h"
#include "Globals.h"
#include "Resource.h"

enum BasicMeshType { MESH_CUBE, MESH_PLANE, MESH_FBX, MESH_SPHERE };

class Mesh
{
public:
	Mesh();
	~Mesh();

	uint CreateBuffer();
	void UnbindBuffer();

	bool SetCubeData(); 

	void CleanMeshData(); 

	BasicMeshType GetType() const;

	void LoadToMemory(); 

	bool SaveAsBinary(); 


public: 

	std::string name = ""; 
	float3 color = {0,0,0};

	float3* vertices = nullptr; 
	int* indices = nullptr;
	float* uvs_cords = nullptr;
	float3* normal_cords = nullptr;

	uint num_vertices = 0;
	uint num_indices = 0;
	uint num_uvs = 0;
	uint num_normals = 0;

	uint vertices_id = -1; 
	uint indices_id = -1;
	uint uvs_id = -1;
	uint normals_id = -1;

	BasicMeshType type; 
};


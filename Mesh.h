#pragma once

#include "MathGeoLib\MathGeoLib.h"
#include "Globals.h"
#include "Resource.h"
#include <string>

enum BasicMeshType { MESH_CUBE, MESH_PLANE, MESH_FBX, MESH_SPHERE };

class Mesh : public Resource
{
public:
	Mesh();
	~Mesh();

	uint CreateBuffer();
	void UnbindBuffer();

	bool SetCubeData(); 
	bool SetVertPlaneData(float2 size = {1,1});

	float3 CenterVertices(float3* center, int num);
	void CleanMeshData(); 

	BasicMeshType GetType() const;

	bool IsLoadedToMemory() const; 

	void LoadToMemory();

public: 

	float3 color = {0,0,0};

	char* load_buffer;

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

	std::string meta_path = ""; 

	BasicMeshType type; 
};


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

	void DrawMesh();

	BasicMeshType GetType();

public: 

	std::string name; 
	float3 color; 

	float3* vertex; 
	int* indices;

	uint num_vertex;
	uint num_indices; 

	uint vertex_id; 
	uint indices_id; 

	BasicMeshType type; 
};


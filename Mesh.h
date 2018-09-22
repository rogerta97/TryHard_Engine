#pragma once

#include "MathGeoLib\MathGeoLib.h"
#include "Globals.h"

enum BasicMeshType { MESH_CUBE, MESH_PLANE };

class Mesh
{
public:
	Mesh();
	~Mesh();

	uint CreateBuffer();
	void BindBuffer(); 
	void UnbindBuffer();

	bool SetCubeData(); 
	bool SetPlaneData(); 

	void DrawMesh();

	BasicMeshType GetType();

private: 

	float3* vertex; 
	int* indices;

	uint num_vertex;
	uint num_index; 

	unsigned int buffer_id; 

	BasicMeshType type; 
};


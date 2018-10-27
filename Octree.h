#pragma once

#include <list>
#include "MathGeoLib\MathGeoLib.h"

#define LIMIT_OCTREE_BUCKET 2

class GameObject; 

class OctreeNode
{
public:
	OctreeNode(AABB box, OctreeNode* parent_node);
	~OctreeNode();

	void Draw();
	void Insert(GameObject* new_go); 
	void Split();

public:

	AABB box;
	OctreeNode * parent;
	OctreeNode * childs[8];
	bool leaf;

	std::list<GameObject*> objects_in_node;
};

class Octree
{
public:
	Octree(int obj_limit);
	~Octree();
	
	void Create(AABB limits);
	void Insert(GameObject* new_go); 
	OctreeNode* GetRoot();
	void Draw(); 

private: 

	OctreeNode* root_node;
	int limit_go; 
};




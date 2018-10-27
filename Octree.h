#pragma once

#include <list>
#include "MathGeoLib\MathGeoLib.h"

class OctreeNode
{
public:
	OctreeNode(AABB box, OctreeNode* parent_node);
	~OctreeNode();

	void Draw();
	void Split();

private:

	AABB box;
	OctreeNode * parent;
	OctreeNode * childs[4];
	bool leaf;
};

class Octree
{
public:
	Octree(int obj_limit);
	~Octree();
	
	void Create(AABB limits);
	OctreeNode* GetRoot();
	void Draw(); 

private: 

	OctreeNode* root_node;
	int limit_go; 
};




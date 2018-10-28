#pragma once

#include <list>
#include "MathGeoLib\MathGeoLib.h"

#define LIMIT_OCTREE_BUCKET 1

class GameObject; 

enum OctreeBehaviour
{
	OCTREE_ADAPTATIVE, 
	OCTREE_FIX
};

class OctreeNode
{
public:
	OctreeNode(AABB box, OctreeNode* parent_node);
	~OctreeNode();

	void Draw();
	void Insert(GameObject* new_go, int& obj_num); 
	void GetObjectIntersections(std::list<GameObject*> inter_list, AABB new_go);
	void CleanUp(); 
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
	
	void Create(AABB limits, bool adaptative);
	void CleanUp();

	void Insert(GameObject* new_go); 
	void GetIntersections(std::list<GameObject*> inter_list, GameObject* new_go);
	
	OctreeNode* GetRoot();
	int GetNumObjects(); 
	void Draw(); 

	bool adaptative;

private: 

	OctreeNode* root_node;	
	int limit_go; 
	int obj_ammount; 
};




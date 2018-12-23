#ifndef _OCTREE_H_
#define _OCTREE_H_

#include <list>
#include "MathGeoLib\MathGeoLib.h"
#include "Globals.h"

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
	OctreeNode(AABB box, OctreeNode* parent_node, bool root);
	~OctreeNode();

	void Draw();
	void Insert(GameObject* new_go, int& obj_num); 
	void GetObjectIntersections(std::list<GameObject*> inter_list, AABB new_go);
	void GetFrustumIntersctions(std::list<UID>& inter_list, Frustum frustum);;
	void CleanUp(); 
	void Split();

public:

	AABB box;
	OctreeNode * parent;
	OctreeNode * childs[8];
	bool leaf;
	bool is_root; 

	int division_lvl;

	std::list<GameObject*> objects_in_node;
};

class Octree
{
public:
	Octree();
	~Octree();
	
	void Create(AABB limits, bool adaptative, int obj_limit);
	void CleanUp();

	bool Insert(GameObject* new_go); 
	bool IsNull();
	void GetIntersections(std::list<GameObject*> inter_list, GameObject* new_go);
	void GetIntersections(std::list<UID>& inter_list, Frustum new_frustum);

	void Recalculate(); 
	
	OctreeNode* GetRoot();
	int GetNumObjects(); 
	void Draw(); 

	bool adaptative;
	bool draw;

private: 

	OctreeNode* root_node;	
	int limit_go; 
	int obj_ammount; 
	int sub_limit;  
	
};

#endif




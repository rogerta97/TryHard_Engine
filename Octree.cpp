#include "Octree.h"
#include "DebugDraw.h"

Octree::Octree(int obj_limit)
{
	root_node = nullptr; 
}

Octree::~Octree()
{
}

void Octree::Create(AABB limits)
{
	root_node = new OctreeNode(limits, nullptr);
}

OctreeNode * Octree::GetRoot()
{
	return root_node;
}

void Octree::Draw()
{
	root_node->Draw(); 
}

OctreeNode::OctreeNode(AABB box, OctreeNode* parent_node)
{
	this->box = box; 
	leaf = true; 
	parent = parent_node; 	
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::Draw()
{
	float3 corners[8]; 
	box.GetCornerPoints(corners);
	DebugDrawBox(corners, Color(0.0f, 1.0f, 1.0f));

	if (leaf == false)
		for (int i = 0; i < 4; i++)		
			childs[i]->Draw(); 
}

void OctreeNode::Split()
{
}

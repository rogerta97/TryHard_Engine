#include "Octree.h"
#include "DebugDraw.h"
#include "Application.h"
#include "GameObject.h"
#include "Functions.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"


#include "mmgr\mmgr.h"


#define OCTREE_DIV_LIMIT 4

Octree::Octree()
{
	root_node = nullptr; 
	obj_ammount = 0;
	sub_limit = OCTREE_DIV_LIMIT;
	
}

Octree::~Octree()
{
}

void Octree::Create(AABB limits, bool adaptative, int obj_limit)
{
	limit_go = obj_limit; 

	CleanUp(); //In case we are redoing the octree

	CONSOLE_LOG("new octree creatred"); 

	root_node = new OctreeNode(limits, nullptr, true);
	this->adaptative = adaptative; 

	obj_ammount = 0; 

	for (auto it = App->scene->static_gameobjects.begin(); it != App->scene->static_gameobjects.end(); it++)
	{
		if (Insert((*it)))
			break; 
	}
}

bool Octree::Insert(GameObject * new_go)
{
	bool ret = false;

	//First we check if it's inside the root node
	ComponentMesh* mesh = (ComponentMesh*)new_go->GetComponent(CMP_MESH);
	ComponentTransform* trans = (ComponentTransform*)new_go->GetComponent(CMP_TRANSFORM); 

	if (mesh == nullptr)
		return false; 

	if (root_node == nullptr)
	{
		CONSOLE_ERROR("Object cant be added before creating the octree.", new_go->GetName().c_str());
		return false; 
	}

	//If it intersects we do the following:
	if (root_node->box.Contains(mesh->bounding_box))
	{
		//Add it to the root node, which will look for the best node recursively
		root_node->Insert(new_go, obj_ammount);
		CONSOLE_LOG("GameObject '%s' added to octree.", new_go->GetName().c_str()); 
		
	}
	else if (adaptative)
	{
		//We need to redo the octree taking into account the new GO 
		int obj_lim = limit_go; 

		//The size of the new box is the greatest value from x, y, or z (this is because AABB should be a cube)
		float3 new_size = mesh->bounding_box.minPoint;

		float higher_distance = GetHigherNumber(new_size.Abs().x, new_size.Abs().y, new_size.Abs().z);

		//Create the new adapted AABB
		float3 max_point = float3(higher_distance + 1, higher_distance + 1, higher_distance + 1);
		float3 min_point = -float3(higher_distance + 1, higher_distance + 1, higher_distance + 1);

		AABB new_bb(min_point, max_point);

		//Create a new Octree
		ret = true; 

		CleanUp();
		Create(new_bb, adaptative, obj_lim);

	}
	else
	{
		CONSOLE_ERROR("Object '%s' is outside Octree limits.", new_go->GetName().c_str());
	}

	return ret; 
}

bool Octree::IsNull()
{
	if(root_node != nullptr)
		return false;

	return true; 
}

void Octree::GetIntersections(std::list<GameObject*> inter_list, GameObject * new_go)
{
	ComponentMesh* mesh = (ComponentMesh*)new_go->GetComponent(CMP_MESH); 
	root_node->GetObjectIntersections(inter_list, mesh->bounding_box);
}

void Octree::GetIntersections(std::list<GameObject*> inter_list, Frustum new_frustum)
{
	//root_node->GetFrustumIntersctions(inter_list, new_frustum);
}

void Octree::Recalculate()
{
	AABB octree_root;

	octree_root.minPoint = { -5, -5, -5 };
	octree_root.maxPoint = { 5, 5, 5 };

	App->scene->octree->Create(octree_root, true, 1);
}

void Octree::CleanUp()
{
	if (root_node != nullptr)
	{
		root_node->CleanUp();
		obj_ammount = 0;
		delete(root_node);
		root_node = nullptr;
	}
}

OctreeNode * Octree::GetRoot()
{
	return root_node;
}

int Octree::GetNumObjects()
{
	return obj_ammount;
}

void Octree::Draw()
{
	if (draw)
	{
		App->renderer3D->UseDebugRenderSettings();
		root_node->Draw();
		App->renderer3D->UseCurrentRenderSettings();
	}	
}

OctreeNode::OctreeNode(AABB box, OctreeNode* parent_node, bool root)
{
	this->box = box; 
	leaf = true; 
	parent = parent_node;
	is_root = root; 
		
	if (parent != nullptr)
		division_lvl = parent->division_lvl + 1;
	else
		division_lvl = 0;
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::Draw()
{
	if (App->scene->octree->IsNull())
		return; 

	float3 corners[8]; 
	box.GetCornerPoints(corners);
	DebugDrawBox(corners, Color(1.0f, 0.6f, 0.0f));

	if (leaf == false)
		for (int i = 0; i < 8; i++)		
			childs[i]->Draw(); 
}

void OctreeNode::Insert(GameObject* new_go, int& num_obj)
{
	ComponentMesh* mesh = (ComponentMesh*)new_go->GetComponent(CMP_MESH); 

	if (mesh == nullptr)
		return; 

	//First we check if it's inside the node
	if (box.Intersects(mesh->bounding_box))
	{
		//If the node is not leaf, we need to Split
		if (leaf == false)
		{
			for (int i = 0; i < 8; i++)
				childs[i]->Insert(new_go, num_obj);
		}		
		else
		{
			if (division_lvl < OCTREE_DIV_LIMIT)
			{
				//We Add the GO normally
				objects_in_node.push_back(new_go);
				num_obj++;

				//If it's leaf, first we check if adding the gameobject would cause a partition
				if (objects_in_node.size() > LIMIT_OCTREE_BUCKET)
				{
					//We need to split and reasign the gameobjects 
					Split();
				}
			}		
		}		
	}
}

void OctreeNode::GetObjectIntersections(std::list<GameObject*> inter_list, AABB new_go_bb)
{
	if (box.Intersects(new_go_bb))
	{
		for (std::list<GameObject*>::const_iterator it = objects_in_node.begin(); it != objects_in_node.end(); ++it)
		{
			if ((*it)->bounding_box->Intersects(new_go_bb))
				inter_list.push_back(*it);

		}

		for (int i = 0; i < 8; i++)
		{
			childs[i]->GetObjectIntersections(inter_list, new_go_bb);
		}
	}
}

void OctreeNode::GetFrustumIntersctions(std::list<GameObject*> inter_list, Frustum frustum)
{
	if (box.Intersects(frustum))
	{
		for (std::list<GameObject*>::const_iterator it = objects_in_node.begin(); it != objects_in_node.end(); ++it)
		{
			if ((*it)->bounding_box != NULL)
			{
				if ((*it)->bounding_box->Intersects(frustum))
					inter_list.push_back(*it);
			}
		}

		for (int i = 0; i < 8; i++)
		{
			childs[i]->GetFrustumIntersctions(inter_list, frustum);
		}
	}
}

void OctreeNode::CleanUp()
{
	if (!leaf)
	{
		for (int i = 0; i < 8; i++)
		{
			childs[i]->CleanUp();
			delete(childs[i]); 
		}
	}

	objects_in_node.clear();
}

void OctreeNode::Split()
{
	//Get the size of the new nodes
	float3 new_size = box.HalfSize();

	// Create the new nodes 
	int children_count = 0; 
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				float3 min_point(box.minPoint.x + z * new_size.x, box.minPoint.y + y * new_size.y, box.minPoint.z + x * new_size.z);
				float3 max_point(min_point.x + new_size.x, min_point.y + new_size.y, min_point.z + new_size.z);

				AABB new_box(min_point, max_point);

				childs[children_count++] = new OctreeNode(new_box, this, false);
			}
		}
	}

	//Now reasign the objects to their new corresponding node (GO's can not lay on a not leaf node)
	for (auto it = objects_in_node.begin(); it != objects_in_node.end(); it++)
	{
		ComponentMesh* object_mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);

		for (int i = 0; i < 8; i++)
		{
			if (childs[i]->box.Intersects(object_mesh->bounding_box))
			{
				if (childs[i]->objects_in_node.size() >= LIMIT_OCTREE_BUCKET)
					childs[i]->Split(); 

				else
					childs[i]->objects_in_node.push_back(*it);
			}
		}
	}

	objects_in_node.clear(); 
	leaf = false; 
}

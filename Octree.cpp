#include "Octree.h"
#include "DebugDraw.h"
#include "Application.h"
#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"

Octree::Octree()
{
	root_node = nullptr; 
	obj_ammount = 0;
}

Octree::~Octree()
{
}

void Octree::Create(AABB limits, bool adaptative, int obj_limit)
{
	limit_go = obj_limit; 

	CleanUp(); //In case we are redoing the octree

	root_node = new OctreeNode(limits, nullptr);
	this->adaptative = adaptative; 

	for (auto it = App->scene->static_gameobjects.begin(); it != App->scene->static_gameobjects.end(); it++)
	{
		Insert((*it)); 
	}
}

void Octree::Insert(GameObject * new_go)
{
	//First we check if it's inside the root node
	ComponentMesh* mesh = (ComponentMesh*)new_go->GetComponent(CMP_MESH);
	ComponentTransform* trans = (ComponentTransform*)new_go->GetComponent(CMP_TRANSFORM); 

	if (mesh == nullptr)
		return; 

	if (root_node == nullptr)
	{
		CONSOLE_ERROR("Object cant be added before creating the octree.", new_go->GetName().c_str());
		return; 
	}

	//If it intersects we do the following:
	if (root_node->box.Intersects(mesh->bounding_box))
	{
		//Add it to the root node, which will look for the best node recursively
		root_node->Insert(new_go, obj_ammount);
		CONSOLE_LOG("GameObject '%s' added to octree.", new_go->GetName().c_str()); 
		
	}
	else if (adaptative)
	{
		//We need to redo the octree taking into account the new GO 
		int obj_lim = limit_go; 

		CleanUp();

		//The size of the new box is the greatest value from x, y, or z (this is because AABB should be a cube)
		float3 new_size = trans->GetPosition().Abs();

		float higher_distance = 0;

		if (new_size.x > higher_distance) higher_distance = new_size.x; 
		if (new_size.y > higher_distance) higher_distance = new_size.y;
		if (new_size.z > higher_distance) higher_distance = new_size.z;

		//Create the new adapted AABB
		float3 max_point = float3(higher_distance, higher_distance, higher_distance);
		float3 min_point = -float3(higher_distance, higher_distance, higher_distance);

		AABB new_bb(min_point, max_point);

		//Create a new Octree
		Create(new_bb, adaptative, obj_lim);

	}
	else
	{
		CONSOLE_ERROR("Object '%s' is outside Octree limits.", new_go->GetName().c_str());
	}
}

void Octree::GetIntersections(std::list<GameObject*> inter_list, GameObject * new_go)
{
	ComponentMesh* mesh = (ComponentMesh*)new_go->GetComponent(CMP_MESH); 
	root_node->GetObjectIntersections(inter_list, mesh->bounding_box);
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
	App->renderer3D->UseDebugRenderSettings();
	root_node->Draw(); 
	App->renderer3D->UseCurrentRenderSettings(); 
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

				childs[children_count++] = new OctreeNode(new_box, this);
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

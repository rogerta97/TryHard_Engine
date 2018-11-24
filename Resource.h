#pragma once

#include "Globals.h"
#include <string>

enum resource_type
{
	RES_MESH,
	RES_TEXTURE,
	RES_MATERIAL, 
	RES_PREFAB, 
	RES_SCENE,
	RES_NULL,
};

class Resource
{
public:
	Resource() { reference_counting = 0; }
	~Resource() {}

	std::string name = "";
	std::string path = "";

	//Setters & Getters
	UID GetUID() const
	{
		return uid;
	}

	void SetUID(UID new_uid)
	{
		uid = new_uid;
	}

	UID GetType() const
	{
		return res_type;
	}

	void SetType(resource_type new_type)
	{
		res_type = new_type;
	}

	virtual bool IsLoadedToMemory() { return false;  }
	virtual void LoadToMemory() {}

	int reference_counting = 0; 

protected:

	UID uid; 
	resource_type res_type; 
	
};
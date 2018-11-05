#pragma once

#include "Globals.h"

enum resource_type
{
	RES_MESH,
	RES_TEXTURE,
	RES_NULL,
};

class Resource
{
public:
	Resource() {}
	~Resource() {}

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

private:

	UID uid; 
	resource_type res_type; 
};
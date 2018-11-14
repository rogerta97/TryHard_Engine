#pragma once

#include "Resource.h"
#include "GameObject.h"

class Prefab :
	public Resource
{
public:
	Prefab();
	~Prefab();

private:

	GameObject * root; 
};


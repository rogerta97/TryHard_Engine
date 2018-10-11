#pragma once

#include "MathGeoLib\MathGeoLib.h"

class Transform
{
public:
	Transform();
	~Transform();

	float3 position; 
	Quat rotation; 
	float3 scale; 	
};


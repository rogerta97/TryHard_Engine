#pragma once

#include "MathGeoLib\MathGeoLib.h"

class Transform
{
public:
	Transform();
	~Transform();

	float3 position; 
	float3 rotation; 
	float3 scale; 	
};


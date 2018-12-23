#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include "MathGeoLib\MathGeoLib.h"

class Transform
{
public:
	Transform();
	~Transform();

	float3 position; 
	Quat rotation; 
	float3 scale; 	

	float3 euler_angles; 

	float3 X, Y, Z; //Axis
};

#endif
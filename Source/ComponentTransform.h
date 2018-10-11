#pragma once

#include "Transform.h"
#include "Component.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform();
	~ComponentTransform();

	bool Start(); 
	bool Update(); 
	bool CleanUp(); 

	//Setters & Getters
	float3 GetPosition() const;
	Quat GetRotation() const; 
	float3 GetScale() const;

	void SetPosition(float3 new_pos);
	void SetRotation(Quat new_pos);
	void SetScale(float3 new_pos);

private:

	Transform transform; 
};


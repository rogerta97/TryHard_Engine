#pragma once

enum CompType {CMP_TRANSFORM, CMP_RENDERER};

class Component {
public:
	Component();
	~Component();

	virtual bool Start(); 
	virtual bool Update(); 

	//Setters & Getters
	CompType GetType() const;
	void SetType(CompType new_type);

private:

	CompType component_type;  
};

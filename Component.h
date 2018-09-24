#pragma once

enum CompType {CMP_TRANSFORM = 0, CMP_RENDERER};

class Component {
public:
	Component();
	~Component();

	virtual bool Start(); 
	virtual bool Update(); 

	//Setters & Getters
	CompType GetType() const;
	void SetType(CompType new_type);

protected:

	CompType component_type;  
};

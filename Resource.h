#pragma once

class Resource
{
public:
	Resource(); 
	~Resource();

	virtual bool Load(); 
	virtual bool Save(); 
};
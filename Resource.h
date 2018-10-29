#pragma once

class Resource
{
public:
	Resource(); 
	~Resource();

	virtual bool SaveAsBinary();
};
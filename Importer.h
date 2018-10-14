#pragma once

enum ImporterType {IMP_MESH};

class Importer
{
public:
	Importer(){}
	~Importer(){}

	virtual bool Start() { return true;  }
	virtual bool Update() { return true; }
	virtual bool CleanUp() { return true; }

	ImporterType imp_type;
	bool load_item; 
};


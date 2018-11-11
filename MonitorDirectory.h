#pragma once

#include "Timer.h"
#include <string>
#include <list>

class MonitorDirectoryNode; 

class MonitorDirectory
{
public:
	MonitorDirectory();
	~MonitorDirectory();

	void Update(); 
	std::list<std::string> GetNewFiles();

	void StartMonitoring(std::string new_node_path);

	MonitorDirectoryNode* GetNode(std::string name); 

	MonitorDirectoryNode* root; 

private:

	Timer update_timer; 
	float update_rate;
};


#pragma once

#include "Timer.h"
#include <string>
#include <list>
#include <vector>

class MonitorDirectoryNode; 

class MonitorDirectory
{
public:
	MonitorDirectory();
	~MonitorDirectory();

	void Update(); 
	std::list<std::string> GetNewFiles();

	std::vector<std::string> nodes;

	std::list<MonitorDirectoryNode*> node_list;

	void StartMonitoring(std::string new_node_path);

	MonitorDirectoryNode* GetNode(std::string name); 

	MonitorDirectoryNode* root; 

private:

	Timer update_timer; 
	float update_rate;
};


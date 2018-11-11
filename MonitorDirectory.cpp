#include "MonitorDirectory.h"
#include "MonitorDirectoryNode.h"

#include "Application.h"
#include "ModuleFileSystem.h"

MonitorDirectory::MonitorDirectory()
{
}

MonitorDirectory::~MonitorDirectory()
{
}

void MonitorDirectory::Update()
{
	GetNewFiles(); 
}

std::list<std::string> MonitorDirectory::GetNewFiles()
{
	std::list<std::string> new_files_list; 

	if (update_timer.Read() >= 500.0f)
	{
		root->GetNewFiles(new_files_list); 
		update_timer.Start();
	}

	return new_files_list; 
}

void MonitorDirectory::StartMonitoring(std::string new_node_path)
{
	root = new MonitorDirectoryNode();
	root->SetPath(new_node_path); 
	root->SetName("Root"); 

	std::vector<std::string> nodes = App->file_system->GetAllFoldersInDirectory(new_node_path.c_str(), true);

	//Create the nodes
	for (auto it = nodes.begin(); it != nodes.end(); it++)
	{
		MonitorDirectoryNode* new_node = new MonitorDirectoryNode();
		new_node->CreateNode(root, (*it)); 
	}

	update_timer.Start();
}

MonitorDirectoryNode * MonitorDirectory::GetNode(std::string name)
{
	MonitorDirectoryNode * node_found = root->GetNode(name);
	return node_found;
}



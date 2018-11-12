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
	if (update_timer.Read() >= 500.0f)
	{
		std::list<std::string> files_added = GetNewFiles();

		for (auto it = files_added.begin(); it != files_added.end(); it++)
		{
			App->resources->ManageNewFolderFile((*it).c_str()); 
			CONSOLE_LOG("File %s was added !!", (*it).c_str());
		}

		update_timer.Start();
	}
}

std::list<std::string> MonitorDirectory::GetNewFiles()
{
	std::list<std::string> new_files_list; 

	root->GetNewFiles(new_files_list); 

	for (auto it = root->childs.begin(); it != root->childs.end(); it++)
	{
		(*it)->GetNewFiles(new_files_list);
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



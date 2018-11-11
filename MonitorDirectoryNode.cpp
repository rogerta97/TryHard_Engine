#include "MonitorDirectoryNode.h"

#include "Application.h"

MonitorDirectoryNode::MonitorDirectoryNode()
{
	parent = nullptr; 
	file_ammount = 0; 
}


MonitorDirectoryNode::~MonitorDirectoryNode()
{
}

void MonitorDirectoryNode::CreateNode(MonitorDirectoryNode * parent, std::string path = "")
{
	this->parent = parent; 

	if (parent != nullptr)
		this->parent->AddChild(this);

	this->path = path; 
	std::vector<std::string> file_list; 

	App->file_system->GetFilesInDirectory(path.c_str(), file_list, false); 
	this->file_ammount = file_list.size();

	name = App->file_system->GetLastPathItem(path.c_str(), false);
}

void MonitorDirectoryNode::AddChild(MonitorDirectoryNode * new_node)
{
	childs.push_back(new_node); 
}

int MonitorDirectoryNode::GetFileAmmount()
{
	return file_ammount;
}

void MonitorDirectoryNode::SetFileAmmount(int new_ammount)
{
	file_ammount = new_ammount; 
}

std::string MonitorDirectoryNode::GetPath()
{
	return path; 
}

void MonitorDirectoryNode::SetPath(std::string new_ammount)
{
	path = new_ammount; 
}

std::string MonitorDirectoryNode::GetName()
{
	return std::string();
}

void MonitorDirectoryNode::SetName(std::string new_name)
{
	name = new_name; 
}

MonitorDirectoryNode* MonitorDirectoryNode::GetNode(std::string name)
{
	if (this->name == name)
		return this; 

	for (auto it = childs.begin(); it != childs.end(); it++)
	{
		(*it)->GetNode(name); 
	}

	return nullptr; 
}

bool MonitorDirectoryNode::IsItemAdded()
{
	std::vector<std::string> files_in_dir;
	App->file_system->GetFilesInDirectory(path.c_str(), files_in_dir, false);

	int file_num = files_in_dir.size(); 

	if (file_num > file_ammount)
		return true; 

	return false;
}

void MonitorDirectoryNode::GetNewFiles(std::list<std::string> new_files_list)
{
	if (IsItemAdded())
	{
		CONSOLE_LOG("ITEM ADDED TO %s BY MONITORING DIRECTORY :)", name.c_str()); 
	}
}

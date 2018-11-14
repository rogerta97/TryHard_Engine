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

	App->file_system->GetFilesInThisDirectory(path.c_str(), file_list, false); 
	this->file_ammount = file_list.size();
	this->curr_files = file_list;

	name = App->file_system->GetLastPathItem(path, false);
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
	


	return false;
}

void MonitorDirectoryNode::GetNewFiles(std::list<std::string>& new_files_list)
{
	std::vector<std::string> files_in_dir;
	App->file_system->GetFilesInThisDirectory(path.c_str(), files_in_dir, false);

	if (files_in_dir.size() > file_ammount)
	{
		for (auto it = files_in_dir.begin(); it != files_in_dir.end(); it++)
		{
			bool contained = false;

			for (auto it2 = curr_files.begin(); it2 != curr_files.end(); it2++)
			{
				if ((*it) == (*it2))
				{
					contained = true;
					break;
				}
			}

			if (contained)
				continue;
			else
			{
				new_files_list.push_back((*it));
				file_ammount = files_in_dir.size();
			}
		}
	}
}

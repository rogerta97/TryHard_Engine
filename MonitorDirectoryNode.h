#ifndef _MONITOR_DIRECTORY_NODE_H_
#define _MONITOR_DIRECTORY_NODE_H_

#include <list>
#include <vector>

class MonitorDirectoryNode
{
public:
	MonitorDirectoryNode();
	~MonitorDirectoryNode();

	void CreateNode(MonitorDirectoryNode* parent, std::string path);

	void AddChild(MonitorDirectoryNode* new_node);

	int GetFileAmmount();
	void SetFileAmmount(int new_ammount); 

	std::string GetPath();
	void SetPath(std::string new_ammount);

	std::string GetName();
	void SetName(std::string new_name);

	MonitorDirectoryNode* GetNode(std::string name);
	bool IsItemAdded();
	void GetNewFiles(std::list<std::string>& new_files_list);

	std::list<MonitorDirectoryNode*> childs;
	std::vector<std::string> curr_files;

private:

	MonitorDirectoryNode * parent; 	

	std::string path = ""; 
	std::string name = ""; 

	int file_ammount;
	
};

#endif


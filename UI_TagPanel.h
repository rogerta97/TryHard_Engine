#ifndef __UI_TAG_PANEL_H__
#define __UI_TAG_PANEL_H__

#include  "UI_Panel.h"

#include <list>
#include <string>

using namespace std; 

class UI_TagPanel : public UI_Panel
{
public:
	UI_TagPanel();
	~UI_TagPanel();

	bool Start();
	bool Update();

	void AddTag(string new_tag); 

	string GetTagByIndex(int index); 

	list<string> tag_list; 
	string tag_to_add; 
};

#endif
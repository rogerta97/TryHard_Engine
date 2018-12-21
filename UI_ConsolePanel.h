#ifndef _UI_CONSOLE_PANEL_H_
#define _UI_CONSOLE_PANEL_H_

#include "UI_Panel.h"

class UI_ConsolePanel : public UI_Panel
{
public:
	UI_ConsolePanel();
	~UI_ConsolePanel();

	bool Start();
	bool Update();

private: 

	bool show_debug; 
	bool show_log;
	bool show_error;

	ImVec4 debug_color; 
	ImVec4 log_color;
	ImVec4 error_color;

};

#endif


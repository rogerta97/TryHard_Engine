#ifndef __UI_CONFIGURATION_PANEL_H__
#define __UI_CONFIGURATION_PANEL_H__


#include "UI_Panel.h"


class UI_ConfigurationPanel : public UI_Panel
{

public:
	UI_ConfigurationPanel();
	~UI_ConfigurationPanel();

	bool Start(); 
	bool Update();

	bool show = false; 


};

#endif

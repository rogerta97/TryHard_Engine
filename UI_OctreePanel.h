#ifndef __UI_OCTREE_PANEL_H__
#define __UI_OCTREE_PANEL_H_

#include "UI_Panel.h"

class Octree; 

class UI_OctreePanel : public UI_Panel
{
public:
	UI_OctreePanel();
	~UI_OctreePanel();

	bool Start();
	bool Update();
	bool CleanUp();

	Octree* octree; 
	float size; 
};

#endif


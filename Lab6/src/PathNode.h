#pragma once
#ifndef __PATH_NODE__
#define __PATH_NODE__

#include "Agent.h"

class PathNode:public Agent
{
public:
	//constructor
	PathNode();

	//destructor
	~PathNode();

	//life-cycle function
	void Draw() override;
	void Update() override;
	void Clean() override;

};






#endif

#pragma once
#ifndef __PATROL_ACTION__
#define __PATROL_ACTION__

#include "ActionNode.h"

class PatrolAction : public ActionNode
{
public:
	PatrolAction();
	virtual ~PatrolAction();

	virtual void Action() override;//define our action here


};



#endif /*defined (__PATROL_ACTION__)*/
